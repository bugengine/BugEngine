from waflib import Task, Options, Build, Utils, Errors, TaskGen, Node, ConfigSet, Context
from waflib.Configure import conf
from waflib.TaskGen import feature, taskgen_method, extension, before_method, after_method
import os
import sys
import importlib
from waflib.Tools import ccroot, c, cxx, winres, c_osx

Build.PROTOCOL = 2
old_log_display = Task.Task.log_display

ccroot.USELIB_VARS['cxx'].add('CLC_CXXFLAGS')


def to_string(self):
    if self.outputs:
        tgt_str = ' '.join([a.name for a in self.outputs][:1])
    else:
        tgt_str = ' '.join([a.name for a in self.inputs][:1])
    task_name = self.__class__.__name__.replace('_task', '')
    return '{%s}%s%s/%s' % (task_name, (12 - len(task_name)) * ' ', self.generator.target, tgt_str)


def log_display(self, bld):
    if not Options.options.silent:
        old_log_display(self, bld)


setattr(Task.Task, '__str__', to_string)
setattr(Task.Task, 'log_display', log_display)
setattr(Task.Task, 'keyword', (lambda self: ''))


def build(bld):
    # type: (Build.BuildContext) -> None
    "Loads main build file as well as the target-specific build file that can declare extra modules"
    if getattr(bld, 'bugengine_variant', None) is None:
        raise Errors.WafError(
            'Call %s %s %s:toolchain:variant\n'
            '  (with toolchain in:\n    %s)\n  (with variant in:\n    %s)' % (
                sys.executable, sys.argv[0], bld.cmd, '\n    '.join(bld.env.ALL_TOOLCHAINS
                                                                    ), '\n    '.join(bld.env.ALL_VARIANTS)
            )
        )
    bld.env = bld.all_envs[bld.bugengine_variant]
    bld.package_env = bld.all_envs['packages']
    bld.package_node = bld.bldnode.parent.parent.make_node('packages')
    bld.platforms = []

    tool_dir = os.path.join(bld.bugenginenode.abspath(), 'mak', 'tools')
    bld.load('cpp_parser', tooldir=[tool_dir])
    bld.load('data', tooldir=[tool_dir])
    bld.load('kernel_ast', tooldir=[tool_dir])
    bld.load('kernel_task', tooldir=[tool_dir])
    bld.load('bin2c', tooldir=[tool_dir])
    bld.load('clir', tooldir=[tool_dir])
    bld.load('ir_compiler', tooldir=[tool_dir])
    bld.env.STATIC = bld.env.STATIC or Options.options.static
    bld.env.DYNAMIC = Options.options.dynamic
    if bld.env.STATIC and bld.env.DYNAMIC:
        raise Errors.WafError('Engine requested to be built both as static and dynamic')
    bld.original_env = bld.env
    bld.common_env = bld.env

    bld.recurse('host/host.py')
    bld.recurse('unit_test.py')
    bld.recurse('install.py')
    for env_name in bld.env.SUB_TOOLCHAINS:
        bld.common_env.append_unique('VALID_PLATFORMS', bld.all_envs[env_name].VALID_PLATFORMS)
    bld.multiarch_envs = [bld.all_envs[envname] for envname in sorted(bld.env.SUB_TOOLCHAINS)] or [bld.env]

    bld.recurse('modules/modules.py')
    bld.recurse('target/target.py')
    bld.recurse('compiler/compiler.py')

    if bld.env.PROJECTS:

        def rc_hook(self, node):
            # type: (TaskGen.task_gen, Node.Node) -> None
            "creates RC hook to silence waf error"
            # pylint: disable=unused-argument
            pass

        if '.rc' not in TaskGen.task_gen.mappings:
            TaskGen.task_gen.mappings['.rc'] = rc_hook


@taskgen_method
def install_directory(self, env, node, local_path, env_variable):
    target_path = os.path.join(self.bld.env.PREFIX, self.bld.__class__.optim, env[env_variable], local_path)
    for n in node.listdir():
        sn = node.make_node(n)
        if os.path.isdir(sn.abspath()):
            self.install_directory(env, sn, os.path.join(local_path, n), env_variable)
        else:
            self.install_files(target_path, [sn])


@taskgen_method
def install_files(self, out_dir, file_list, chmod=Utils.O644):
    self.deploy_files(out_dir, file_list, chmod)


@taskgen_method
def install_as(self, target_path, file, chmod=Utils.O644):
    self.deploy_as(target_path, file, chmod)


@taskgen_method
def deploy_files(self, out_dir, file_list, chmod=Utils.O644):
    try:
        install_task = self.bug_install_task
    except AttributeError:
        install_task = self.bug_install_task = self.create_task('install', [], [])
        install_task.install_step = []
    install_task.inputs += file_list
    for f in file_list:
        install_task.install_step.append((f, os.path.join(out_dir, f.name), chmod))


@taskgen_method
def deploy_as(self, target_path, file, chmod=Utils.O644):
    try:
        install_task = self.bug_install_task
    except AttributeError:
        install_task = self.bug_install_task = self.create_task('install', [], [])
        install_task.install_step = []
    install_task.inputs.append(file)
    install_task.install_step.append((file, target_path, chmod))


@feature('bugengine:c', 'bugengine:cxx')
def set_optim_define(self):
    o = getattr(self.bld, 'optim', None)
    if o:
        self.env.append_unique('DEFINES', ['BE_%s' % o.upper()])


@feature('bugengine:cxx')
def set_building_name_inherits(self):
    seen = set([])
    use = getattr(self, 'use', [])[:]
    while (use):
        x = use.pop()
        if x in seen:
            continue
        seen.add(x)
        try:
            y = self.bld.get_tgen_by_name(x)
        except:
            pass
        else:
            if 'cxxobjects' in y.features:
                use += getattr(y, 'use', [])
                self.env.append_unique('DEFINES', 'building_%s' % y.safe_target_name)


@feature('bugengine:launcher', 'bugengine:unit_test', 'bugengine:python_module')
@before_method('apply_link')
@before_method('process_use')
def static_dependencies(self):
    if self.bld.env.STATIC:
        for g in self.bld.groups:
            for task_gen in g:
                if not isinstance(task_gen, TaskGen.task_gen):
                    continue
                if (
                    'bugengine:kernel' in task_gen.features or 'bugengine:plugin' in task_gen.features
                ) and 'cxx' in task_gen.features:
                    task_gen.post()
                    if task_gen.env.TOOLCHAIN == self.env.TOOLCHAIN:
                        self.use.append(task_gen.target)


@feature('bugengine:launcher_static')
@before_method('apply_link')
def rename_executable(self):
    self.target = self.real_target


@feature('cxxobjects')
def objects_feature(task):
    pass


@feature('Makefile')
def makefile_feature(task):
    pass


@feature('bugengine:masterfiles:off', 'bugengine:masterfiles:folder')
def masterfiles_feature(task):
    pass


@feature('bugengine:warnings:off', 'bugengine:nortc')
def warning_feature(task):
    pass


@feature('bugengine:export_all')
@before_method('process_source')
def process_export_all_flag(self):
    self.env.append_unique('CFLAGS', self.env.CFLAGS_exportall)
    self.env.append_unique('CXXFLAGS', self.env.CXXFLAGS_exportall)


@feature('bugengine:c', 'bugengine:cxx')
@before_method('process_source')
def set_extra_flags(self):
    for f in getattr(self, 'features', []):
        self.env.append_unique('CLT_CXXFLAGS', self.env['CLT_CXXFLAGS_%s' % f])
        self.env.append_unique('CPPFLAGS', self.env['CPPFLAGS_%s' % f])
        self.env.append_unique('CFLAGS', self.env['CFLAGS_%s' % f])
        self.env.append_unique('CXXFLAGS', self.env['CXXFLAGS_%s' % f])
        self.env.append_unique('LINKFLAGS', self.env['LINKFLAGS_%s' % f])
        self.env.append_unique('LIB', self.env['LIB_%s' % f])
        self.env.append_unique('STLIB', self.env['STLIB_%s' % f])


@taskgen_method
def check_use_taskgens(self):
    """
    Checks all names in 'use' are valid task generators, or move them to uselib
    """
    for var in 'use', 'private_use':
        use = getattr(self, var, [])
        for name in use[:]:
            try:
                y = self.bld.get_tgen_by_name(name)
            except:
                use.remove(name)
                try:
                    self.uselib.append(name)
                except AttributeError:
                    self.uselib = [name]


@feature('bugengine:c', 'bugengine:cxx')
def process_warning_flags(self):
    warning_flag_name = 'warnnone' if 'bugengine:warnings:off' in self.features else 'warnall'
    for var in self.get_uselib_vars():
        self.env.append_value(var, self.env['%s_%s' % (var, warning_flag_name)])


@taskgen_method
def process_use_flags(self):
    if 'bugengine:nortc' not in self.features:
        self.env.append_unique('CFLAGS', self.env.CFLAGS_rtc)
        self.env.append_unique('CFLAGS_debug', self.env.CFLAGS_debug_rtc)
        self.env.append_unique('CFLAGS_profile', self.env.CFLAGS_profile_rtc)
        self.env.append_unique('CFLAGS_final', self.env.CFLAGS_final_rtc)
        self.env.append_unique('CXXFLAGS', self.env.CXXFLAGS_rtc)
        self.env.append_unique('CXXFLAGS_debug', self.env.CXXFLAGS_debug_rtc)
        self.env.append_unique('CXXFLAGS_profile', self.env.CXXFLAGS_profile_rtc)
        self.env.append_unique('CXXFLAGS_final', self.env.CXXFLAGS_final_rtc)
    else:
        self.env.append_unique('CFLAGS', self.env.CFLAGS_nortc)
        self.env.append_unique('CFLAGS_debug', self.env.CFLAGS_debug_nortc)
        self.env.append_unique('CFLAGS_profile', self.env.CFLAGS_profile_nortc)
        self.env.append_unique('CFLAGS_final', self.env.CFLAGS_final_nortc)
        self.env.append_unique('CXXFLAGS', self.env.CXXFLAGS_nortc)
        self.env.append_unique('CXXFLAGS_debug', self.env.CXXFLAGS_debug_nortc)
        self.env.append_unique('CXXFLAGS_profile', self.env.CXXFLAGS_profile_nortc)
        self.env.append_unique('CXXFLAGS_final', self.env.CXXFLAGS_final_nortc)
    dependencies = [self.bld.get_tgen_by_name(i) for i in getattr(self, 'use', []) + getattr(self, 'private_use', [])]
    seen = set([self])
    while dependencies:
        dep = dependencies.pop(0)
        if dep not in seen:
            seen.add(dep)
            dep.post()
            dependencies += [self.bld.get_tgen_by_name(i) for i in getattr(dep, 'use', [])]
            for var in self.get_uselib_vars():
                value = getattr(dep, 'export_%s' % var.lower(), [])
                self.env.append_value(var, Utils.to_list(value))


@feature('cxxshlib', 'cshlib')
def be_build_dll(self):
    try:
        self.export_defines.append('be_dll_%s' % self.safe_target_name)
    except AttributeError:
        self.export_defines = ['be_dll_%s' % self.safe_target_name]


@feature('cxxobjects', 'cobjects')
def be_build_objects(self):
    if not self.env.STATIC:
        try:
            self.export_defines.append('be_dll_%s' % self.safe_target_name)
        except AttributeError:
            self.export_defines = ['be_dll_%s' % self.safe_target_name]


@taskgen_method
def process_use_link(self):
    link_task = getattr(self, 'link_task', None)
    if link_task:
        dependencies = [
            (self.bld.get_tgen_by_name(i), True) for i in getattr(self, 'use', []) + getattr(self, 'private_use', [])
        ]
        all_deps = dependencies[::]
        seen = set([self])
        while dependencies:
            dep, link_objects = dependencies.pop(0)
            if dep not in seen:
                seen.add(dep)
                dep.post()
                link_objects = link_objects and not hasattr(dep, 'link_task')
                new_deps = [self.bld.get_tgen_by_name(i) for i in dep.use + getattr(dep, 'private_use', [])]
                for d in new_deps:
                    try:
                        all_deps.remove((d, link_objects))
                    except ValueError:
                        pass
                    all_deps.append((d, link_objects))
                dependencies += [(i, link_objects) for i in new_deps]
        for d, link_objects in all_deps:
            for var in 'LIB', 'LIBPATH', 'STLIB', 'STLIBPATH', 'LINKFLAGS', 'FRAMEWORK':
                value = getattr(d, 'export_%s' % var.lower(), [])
                self.env.append_value(var, Utils.to_list(value))
            if 'cxxstlib' in d.features or 'cstlib' in d.features:
                self.env.append_value('STLIB', [os.path.basename(d.target)])
                link_task.dep_nodes.extend(d.link_task.outputs)
                tmp_path = d.link_task.outputs[0].parent.path_from(self.bld.bldnode)
                self.env.append_value('STLIBPATH', [tmp_path])
            elif 'cxxshlib' in d.features or 'cshlib' in d.features:
                self.env.append_value('LIB', [os.path.basename(d.target)])
                link_task.dep_nodes.extend(d.link_task.outputs)
                tmp_path = d.link_task.outputs[0].parent.path_from(self.bld.bldnode)
                self.env.append_value('LIBPATH', [tmp_path])
            elif link_objects and ('cxxobjects' in d.features or 'cobjects' in d.features):
                self.add_objects_from_tgen(d)


@taskgen_method
def process_use(self):
    self.check_use_taskgens()
    self.process_use_link()
    self.process_use_flags()
    self.uselib = Utils.to_list(getattr(self, 'uselib', []))
    for x in getattr(self, 'use', []):
        y = self.bld.get_tgen_by_name(x)
        for k in self.to_list(getattr(y, 'uselib', [])):
            if not self.env['STLIB_' + k] and not k in self.uselib:
                self.uselib.append(k)


@feature('bugengine:c', 'bugengine:cxx')
@before_method('filter_sources')
def gather_extra_source(self):
    preprocess = getattr(self, 'preprocess', None)
    if preprocess:
        preprocess.post()
        self.source += getattr(preprocess, 'out_sources', [])


def _make_bld_node(self, node, category, path, name):
    node = node.make_node(category)
    if not path:
        node = node.make_node(name)
    elif isinstance(path, Node.Node):
        if path.is_child_of(self.bld.bldnode):
            out_dir = path.path_from(self.bld.bldnode)
            # skip variant
            #out_dir = out_dir[out_dir.find(os.path.sep)+1:]
            # skip optim
            #out_dir = out_dir[out_dir.find(os.path.sep)+1:]
            # skip target
            out_dir = out_dir[out_dir.find(os.path.sep) + 1:]
            # skip category
            out_dir = out_dir[out_dir.find(os.path.sep) + 1:]
            node = node.make_node(out_dir)
            node = node.make_node(name)
        elif path.is_child_of(self.bld.bldnode.parent.parent):
            out_dir = path.path_from(self.bld.bldnode.parent.parent)
            # skip variant
            #out_dir = out_dir[out_dir.find(os.path.sep)+1:]
            # skip optim
            #out_dir = out_dir[out_dir.find(os.path.sep)+1:]
            # skip target
            out_dir = out_dir[out_dir.find(os.path.sep) + 1:]
            # skip category
            out_dir = out_dir[out_dir.find(os.path.sep) + 1:]
            node = node.make_node(out_dir)
            node = node.make_node(name)
        else:
            for source_node in self.source_nodes:
                if path.is_child_of(source_node):
                    out_dir = path.path_from(source_node)
                    break
            else:
                out_dir = path.path_from(self.path)
            while out_dir[0] == '.':
                out_dir = out_dir[out_dir.find(os.path.sep) + 1:]
            node = node.make_node(out_dir)
            node = node.make_node(name)
    else:
        node = node.make_node(path)
        node = node.make_node(name)
    if not self.env.PROJECTS:
        node.parent.mkdir()
    return node


@taskgen_method
def make_root_bld_node(self, category, path, name):
    """
        constructs a path from the build node:
            build_node/variant/optim/category/path/name
    """
    return _make_bld_node(self, self.bld.bldnode, category, path, name)


@taskgen_method
def make_bld_node(self, category, path, name):
    """
        constructs a path from the target build node:
            build_node/variant/optim/target/category/path/name
    """
    return _make_bld_node(self, self.bld.bldnode.make_node(self.target), category, path, name)


@taskgen_method
@feature('*')
@before_method('process_source')
def filter_sources(self):
    self.objc = False
    self.source = self.to_nodes(getattr(self, 'source', []))
    if self.env.PROJECTS:
        return
    sources = []
    for file in self.source:
        basename, ext = os.path.splitext(file.name)
        add_platform = True
        add_arch = True
        add_compiler = True
        platform = file.name.find('-p=')
        if platform != -1:
            add_platform = False
            platforms = basename[platform + 3:].split(',')
            for p in platforms:
                add_platform = add_platform or p in self.env.VALID_PLATFORMS
        arch = file.name.find('-a=')
        if arch != -1:
            add_arch = False
            architectures = basename[arch + 3:].split(',')
            for a in architectures:
                add_arch = add_arch or a in self.env.VALID_ARCHITECTURES
        compiler = file.name.find('-c=')
        if compiler != -1:
            add_compiler = False
            compilers = basename[compiler + 3:].split(',')
            for c in compilers:
                add_compiler = add_compiler or c == self.env.COMPILER_NAME
        node = file.parent
        while add_platform and add_arch and add_compiler and node and node != self.path and node != self.bld.srcnode:
            if node.name.startswith('platform='):
                add_platform = False
                platforms = node.name[9:].split(',')
                for p in platforms:
                    add_platform = add_platform or p in self.bld.env.VALID_PLATFORMS
            elif node.name.startswith('arch='):
                add_arch = False
                architectures = node.name[5:].split(',')
                for a in architectures:
                    add_arch = add_arch or a in self.env.VALID_ARCHITECTURES
            elif node.name.startswith('compiler='):
                add_compiler = False
                compilers = node.name[9:].split(',')
                for c in compilers:
                    add_compiler = add_compiler or c == self.env.COMPILER_NAME
            elif node.parent.name == 'extra' and node.parent.parent == self.bld.bugenginenode:
                add_platform = node.name in self.bld.env.VALID_PLATFORMS
            node = node.parent
        if add_platform and add_arch and add_compiler:
            sources.append(file)
            if ext in ['.m', '.mm']:
                self.objc = True
    self.source = sources


def create_master_file(task):
    with open(task.outputs[0].abspath(), 'w') as f:
        for src in task.inputs:
            f.write('#include "%s"\n' % src.path_from(task.generator.bld.srcnode).replace('\\', '/'))


def master_sig_deps(task):
    for f in sorted([i.srcpath().encode('utf8') for i in task.inputs]):
        task.m.update(f)
        task.m.update(b';')


MasterTask = Task.task_factory("master", create_master_file, color='CYAN')
MasterTask.sig_explicit_deps = master_sig_deps


@taskgen_method
def create_compiled_task(self, name, node):
    """
    Create the compilation task: c, cxx, asm, etc. The output node is created automatically (object file with a typical **.o** extension).
    The task is appended to the list *compiled_tasks* which is then used by :py:func:`waflib.Tools.ccroot.apply_link`

    :param name: name of the task class
    :type name: string
    :param node: the file to compile
    :type node: :py:class:`waflib.Node.Node`
    :return: The task created
    :rtype: :py:class:`waflib.Task.Task`
    """
    out = self.make_bld_node('obj', node.parent, node.name[:node.name.rfind('.')] + '.o')
    task = self.create_task(name, node, out)
    extra_env = self.env['%s_env' % name]
    if extra_env:
        if not task.env.env:
            task.env.env = dict(os.environ)
        for k, v in extra_env:
            task.env.env[k] = v
    try:
        self.compiled_tasks.append(task)
    except AttributeError:
        self.compiled_tasks = [task]
    return task


@extension('.def')
def def_file(self, node):
    try:
        self.def_files.append(node)
    except Exception:
        self.def_files = [node]


@extension('.rc')
def rc_file(self, node):
    """
    Bind the .rc extension to a winrc task
    """
    if not self.env.WINRC:
        return
    obj_ext = '.o'
    if self.env['WINRC_TGT_F'] == '/fo':
        obj_ext = '.res'
    out = self.make_bld_node('obj', node.parent, node.name[:node.name.rfind('.')] + obj_ext)
    rctask = self.create_task('winrc', node, out)
    rctask.env.PATH = []
    try:
        self.compiled_tasks.append(rctask)
    except AttributeError:
        self.compiled_tasks = [rctask]


@extension('.c', '.m')
def c_hook(self, node):
    if 'bugengine:c' in self.features:
        if 'bugengine:masterfiles:folder' in self.features and not Options.options.nomaster:
            try:
                mastertask_c = self.mastertasks_c_folders[node.parent]
                mastertask_c.set_inputs([node])
            except AttributeError:
                output = self.make_bld_node(
                    'src', None, 'master-c-%s-0.%s' % (node.parent.name, self.objc and 'm' or 'c')
                )
                mastertask_c = self.create_task('master', [node], [output])
                self.mastertasks_c_folders = {node.parent: mastertask_c}
                self.create_compiled_task('c', output)
            except KeyError:
                output = self.make_bld_node(
                    'src', None,
                    'master-c-%s-%d.%s' % (node.parent.name, len(self.mastertasks_c_folders), self.objc and 'm' or 'c')
                )
                mastertask_c = self.create_task('master', [node], [output])
                self.mastertasks_c_folders[node.parent] = mastertask_c
                self.create_compiled_task('c', output)
        elif 'bugengine:masterfiles:off' not in self.features and not Options.options.nomaster:
            try:
                mastertask_c = self.mastertasks_c[-1]
                if len(mastertask_c.inputs) <= 10:
                    mastertask_c.set_inputs([node])
                else:
                    output = self.make_bld_node(
                        'src', None, 'master-c-%d.%s' % (len(self.mastertasks_c), self.objc and 'm' or 'c')
                    )
                    mastertask_c = self.create_task('master', [node], [output])
                    self.mastertasks_c.append(mastertask_c)
                    self.create_compiled_task('c', output)
            except:
                output = self.make_bld_node('src', None, 'master-c-0.%s' % (self.objc and 'm' or 'c'))
                mastertask_c = self.create_task('master', [node], [output])
                self.mastertasks_c = [mastertask_c]
                self.create_compiled_task('c', output)
        else:
            self.create_compiled_task('c', node)
    else:
        self.create_compiled_task('c', node)


@extension('.cc', '.cxx', '.cpp', '.mm')
def cc_hook(self, node):
    if 'bugengine:cxx' in self.features:
        if 'bugengine:masterfiles:folder' in self.features and not Options.options.nomaster:
            try:
                mastertask_cxx = self.mastertasks_cxx_folders[node.parent]
                mastertask_cxx.set_inputs([node])
            except AttributeError:
                output = self.make_bld_node(
                    'src', None, 'master-c-%s-0.%s' % (node.parent.name, self.objc and 'mm' or 'cc')
                )
                mastertask_cxx = self.create_task('master', [node], [output])
                self.mastertasks_cxx_folders = {node.parent: mastertask_cxx}
                self.create_compiled_task('cxx', output)
            except KeyError:
                output = self.make_bld_node(
                    'src', None, 'master-c-%s-%d.%s' %
                    (node.parent.name, len(self.mastertasks_cxx_folders), self.objc and 'mm' or 'cc')
                )
                mastertask_cxx = self.create_task('master', [node], [output])
                self.mastertasks_cxx_folders[node.parent] = mastertask_cxx
                self.create_compiled_task('cxx', output)
        elif 'bugengine:masterfiles:off' not in self.features and not Options.options.nomaster:
            if node.name.endswith('-instances.cc'):
                try:
                    self.instancetask_cxx.set_inputs([node])
                except:
                    output = self.make_bld_node('src', None, 'instances-master-cxx.%s' % (self.objc and 'mm' or 'cc'))
                    self.instancetask_cxx = self.create_task('master', [node], [output])
                    self.create_compiled_task('cxx', output)
            else:
                try:
                    mastertask_cxx = self.mastertasks_cxx[-1]
                    if len(mastertask_cxx.inputs) <= 10:
                        mastertask_cxx.set_inputs([node])
                    else:
                        output = self.make_bld_node(
                            'src', None, 'master-cxx-%d.%s' % (len(self.mastertasks_cxx), self.objc and 'mm' or 'cc')
                        )
                        mastertask_cxx = self.create_task('master', [node], [output])
                        self.mastertasks_cxx.append(mastertask_cxx)
                        self.create_compiled_task('cxx', output)
                except:
                    output = self.make_bld_node('src', None, 'master-cxx-0.%s' % (self.objc and 'mm' or 'cc'))
                    mastertask_cxx = self.create_task('master', [node], [output])
                    self.mastertasks_cxx = [mastertask_cxx]
                    self.create_compiled_task('cxx', output)
        else:
            self.create_compiled_task('cxx', node)
    else:
        self.create_compiled_task('cxx', node)


@feature('bugengine:c', 'bugengine:cxx')
@after_method('apply_incpaths')
def incpath_master(self):
    if not Options.options.nomaster and 'bugengine:masterfiles:off' not in self.features:
        self.env.INCPATHS = [self.bld.srcnode.abspath()] + self.env.INCPATHS


@feature('c', 'cxx')
@after_method('process_source')
def apply_link(self):
    for x in self.features:
        if x == 'cprogram' and 'cxx' in self.features: # limited compat
            x = 'cxxprogram'
        elif x == 'cshlib' and 'cxx' in self.features:
            x = 'cxxshlib'
        if x in Task.classes:
            if issubclass(Task.classes[x], ccroot.link_task):
                link = x
                break
    else:
        return

    objs = [t.outputs[0] for t in getattr(self, 'compiled_tasks', [])]
    self.link_task = self.create_task(link, objs)
    pattern = self.env['%s_PATTERN' % link]
    if not pattern:
        pattern = '%s'
    path, name = os.path.split(self.target)
    out_node = self.make_root_bld_node('bin', None, os.path.join(path, pattern % name))
    self.link_task.set_outputs(out_node)


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('apply_link')
@before_method('install_step')
def set_postlink_task(self):
    self.postlink_task = self.link_task


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('set_postlink_task')
def install_step(self):
    pass


@feature('bugengine:c', 'bugengine:cxx')
def set_macosx_deployment_target(self):
    pass


def exec_command_objcopy(self, *k, **kw):
    if isinstance(k[0], list):
        lst = []
        carry = ''
        for a in k[0]:
            if a[-1] == '=':
                carry = a
            else:
                lst.append(carry + a)
                carry = ''
            k = [lst]
    return self.generator.bld.exec_command(*k, **kw)


Task.task_factory('dbg_copy', '${OBJCOPY} --only-keep-debug ${SRC} ${TGT[0].abspath()}', color='YELLOW')
dbg_strip_cls = Task.task_factory(
    'dbg_strip',
    '${OBJCOPY} --strip-all --add-gnu-debuglink=${SRC[0].name} ${SRC[1].abspath()} ${TGT[0].abspath()}',
    color='YELLOW'
)
dbg_strip_cls.exec_command = exec_command_objcopy


@taskgen_method
def strip_debug_info_impl(self):
    if self.env.STRIP_BINARY and self.env.STRIP and self.env.OBJCOPY:
        try:
            optim = self.bld.optim
        except AttributeError:
            return
        else:
            self.full_link_task = self.link_task
            full_link = self.full_link_task.outputs[0]
            out_dir = full_link.parent.make_node('post-link')
            out_dir.mkdir()
            debug_prog = out_dir.make_node(full_link.name + '.debug')
            stripped_linked_prog = out_dir.make_node(full_link.name)

            self.dbg_copy_task = self.create_task('dbg_copy', [full_link], [debug_prog])
            self.dbg_strip_task = self.create_task('dbg_strip', [debug_prog, full_link], [stripped_linked_prog])
            self.dbg_strip_task.cwd = out_dir.abspath()
            self.postlink_task = self.dbg_strip_task

            if 'bugengine:plugin' in self.features:
                out_path = self.env.DEPLOY_PLUGINDIR
            elif 'bugengine:kernel' in self.features:
                out_path = self.env.DEPLOY_KERNELDIR
            elif 'bugengine:shared_lib' in self.features:
                out_path = self.env.DEPLOY_RUNBINDIR
            elif 'bugengine:launcher' in self.features:
                out_path = self.env.DEPLOY_BINDIR
            else:
                return

            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path), [debug_prog])


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@before_method('install_step')
@after_method('set_postlink_task')
def strip_debug_info(self):
    if not self.env.SUBARCH:
        self.strip_debug_info_impl()


@feature('c', 'cxx', 'd', 'asm', 'fc', 'includes')
@after_method('propagate_uselib_vars', 'process_source')
def apply_incpaths(self):
    """
    Task generator method that processes the attribute *includes*::

        tg = bld(features='includes', includes='.')

    The folders only need to be relative to the current directory, the equivalent build directory is
    added automatically (for headers created in the build directory). This enable using a build directory
    or not (``top == out``).

    This method will add a list of nodes read by :py:func:`waflib.Tools.ccroot.to_incnodes` in ``tg.env.INCPATHS``,
    and the list of include paths in ``tg.env.INCLUDES``.
    """

    lst = self.to_incnodes(self.to_list(getattr(self, 'includes', [])) + self.env.INCLUDES)
    self.includes_nodes = lst
    self.env.INCPATHS = [x.abspath() for x in lst]
