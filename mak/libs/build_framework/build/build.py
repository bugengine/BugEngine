from waflib import Task, Options, Build, Utils, Errors, TaskGen, Node, ConfigSet, Context
from waflib.Configure import conf
from waflib.TaskGen import feature, taskgen_method, extension, before_method, after_method
import os
import sys
import importlib
from waflib.Tools import ccroot, c, cxx, winres, c_osx

Build.PROTOCOL = 2
old_log_display = Task.Task.log_display


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
    if not bld.env.PROJECTS:
        if getattr(bld, 'bugengine_variant', None) is None:
            raise Errors.WafError(
                'Call %s %s %s:toolchain:variant\n'
                '  (with toolchain in:\n    %s)\n  (with variant in:\n    %s)' % (
                    sys.executable, sys.argv[0], bld.cmd,
                    '\n    '.join(bld.env.ALL_TOOLCHAINS),
                    '\n    '.join(bld.env.ALL_VARIANTS)
                )
            )
        bld.env = bld.all_envs[bld.bugengine_variant]
    bld.platforms = []

    tool_dir = os.path.join(bld.bugenginenode.abspath(), 'mak', 'tools')
    bld.load('cpp_parser', tooldir=[tool_dir])
    bld.load('data', tooldir=[tool_dir])
    bld.load('kernel_ast', tooldir=[tool_dir])
    bld.load('kernel_task', tooldir=[tool_dir])
    bld.env.STATIC = bld.env.STATIC or Options.options.static
    bld.env.DYNAMIC = Options.options.dynamic
    if bld.env.STATIC and bld.env.DYNAMIC:
        raise Errors.WafError('Engine requested to be built both as static and dynamic')
    bld.original_env = bld.env

    bld.recurse('host/host.py')
    bld.recurse('install.py')
    for env_name in bld.env.SUB_TOOLCHAINS:
        bld.common_env.append_unique('VALID_PLATFORMS', bld.all_envs[env_name].VALID_PLATFORMS)
    bld.multiarch_envs = [bld.all_envs[envname] for envname in bld.env.SUB_TOOLCHAINS] or [bld.env]

    bld.recurse('modules/modules.py')
    bld.recurse('target/target.py')
    bld.recurse('compiler/compiler.py')
    bld.common_env = bld.env

    if bld.env.PROJECTS:
        def rc_hook(self, node):
            # type: (TaskGen.task_gen, Node.Node) -> None
            "creates RC hook to silence waf error"
            # pylint: disable=unused-argument
            pass

        if '.rc' not in TaskGen.task_gen.mappings:
            TaskGen.task_gen.mappings['.rc'] = rc_hook


@taskgen_method
def deploy_directory(self, env, node, local_path, env_variable):
    target_path = os.path.join(self.bld.env.PREFIX, self.bld.__class__.optim, env[env_variable], local_path)
    for n in node.listdir():
        sn = node.make_node(n)
        if os.path.isdir(sn.abspath()):
            self.deploy_directory(env, sn, os.path.join(local_path, n), env_variable)
        else:
            self.install_files(target_path, [sn])


@taskgen_method
def install_files(self, out_dir, file_list, chmod=Utils.O644):
    try:
        install_task = self.bug_install_task
    except AttributeError:
        install_task = self.bug_install_task = self.create_task('install', [], [])
        install_task.install_step = []
    install_task.inputs += file_list
    for f in file_list:
        install_task.install_step.append((f, os.path.join(out_dir, f.name), chmod))


@taskgen_method
def install_as(self, target_path, file, chmod=Utils.O644):
    try:
        install_task = self.bug_install_task
    except AttributeError:
        install_task = self.bug_install_task = self.create_task('install', [], [])
        install_task.install_step = []
    install_task.inputs.append(file)
    install_task.install_step.append((file, target_path, chmod))


@feature('c', 'cxx')
def set_optim_define(self):
    o = getattr(self.bld, 'optim', None)
    if o:
        self.env.append_unique('DEFINES', ['BE_%s' % o.upper()])


@feature('cxx')
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
                self.env.append_unique('DEFINES', 'building_%s' % y.target_name.split('.')[-1])


@feature('launcher', 'python_module')
@before_method('apply_link')
@before_method('process_use')
def static_dependencies(self):
    if self.bld.env.STATIC:
        for g in self.bld.groups:
            for task_gen in g:
                if not isinstance(task_gen, TaskGen.task_gen):
                    continue
                if ('kernel' in task_gen.features or 'plugin' in task_gen.features) and 'cxx' in task_gen.features:
                    task_gen.post()
                    if task_gen.env.TOOLCHAIN == self.env.TOOLCHAIN:
                        self.use.append(task_gen.target)


@feature('launcher_static')
@before_method('apply_link')
def rename_executable(self):
    self.target = self.real_target


@feature('launcher')
def launcher_feature(task):
    pass


@feature('cxxobjects')
def objects_feature(task):
    pass


@feature('Makefile')
def makefile_feature(task):
    task_roots = [source_node.abspath() for source_node in task.source_nodes]
    project_root = task.bld.srcnode.abspath()
    env = ConfigSet.ConfigSet()
    env.load(os.path.join(Context.top_dir, Options.lockfile))
    seen = set([])
    for module in sys.modules.values():
        try:
            if module.__file__.startswith(project_root):
                env.files.append(module.__file__)
        except AttributeError as e:
            pass
    for f in env.files:
        for task_root in task_roots:
            if f.startswith(task_root):
                break
        else:
            f = f[len(project_root) + 1:]
            if sys.version_info.major < 3:
                base_name, ext = os.path.splitext(f)
                if ext == '.pyc':
                    f = base_name + '.py'
            else:
                try:
                    f = importlib.util.source_from_cache(f)
                except ValueError:
                    pass
            if os.path.isfile(f):
                if f not in seen:
                    task.source_nodes.append(task.bld.srcnode.make_node(f))
                    seen.add(f)


@feature('cxx')
@before_method('process_source')
def process_export_all_flag(self):
    if getattr(self, 'export_all', False):
        self.env = self.env.derive()
        self.env.append_unique('CFLAGS', self.env.CFLAGS_exportall)
        self.env.append_unique('CXXFLAGS', self.env.CXXFLAGS_exportall)


@feature('cxx')
@before_method('process_source')
def set_extra_flags(self):
    for f in getattr(self, 'extra_use', []) + getattr(self, 'features', []):
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


@taskgen_method
def process_use_flags(self):
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


@taskgen_method
def process_use_link(self):
    link_task = getattr(self, 'link_task', None)
    if link_task:
        dependencies = [(self.bld.get_tgen_by_name(i), True) for i in self.use + getattr(self, 'private_use', [])]
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
    self.process_use_flags()
    self.process_use_link()
    self.uselib = Utils.to_list(getattr(self, 'uselib', []))
    for x in getattr(self, 'use', []):
        y = self.bld.get_tgen_by_name(x)
        for k in self.to_list(getattr(y, 'uselib', [])):
            if not self.env['STLIB_' + k] and not k in self.uselib:
                self.uselib.append(k)


@feature('cxx')
@before_method('filter_sources')
def gather_extra_source(self):
    preprocess = getattr(self, 'preprocess', None)
    if preprocess:
        preprocess.post()
        self.source += getattr(preprocess, 'out_sources', [])


@taskgen_method
def make_bld_node(self, category, path, name):
    """
        constructs a path from the build node:
            build_node/variant/optim/target/category/path/name
    """
    bldnode = self.bld.bldnode
    node = bldnode.make_node(self.target).make_node(category)
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
@feature('*')
@before_method('process_source')
def filter_sources(self):
    self.objc = False
    self.source = self.to_nodes(getattr(self, 'source', []))
    sources = []
    for file in self.source:
        basename, ext = os.path.splitext(file.name)
        add_platform = True
        add_arch = True
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
        node = file.parent
        while add_platform and add_arch and node and node != self.path and node != self.bld.srcnode:
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
            elif node.parent.name == 'extra' and node.parent.parent == self.bld.bugenginenode:
                add_platform = node.name in self.bld.env.VALID_PLATFORMS
            node = node.parent
        if add_platform and add_arch:
            sources.append(file)
            if ext in ['.m', '.mm']:
                self.objc = True
    self.source = sources


def create_master_file(task):
    with open(task.outputs[0].abspath(), 'w') as f:
        for src in task.inputs:
            f.write('#include "%s"\n' % src.path_from(task.generator.bld.bldnode).replace('\\', '/'))


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
    obj_ext = '.o'
    if self.env['WINRC_TGT_F'] == '/fo':
        obj_ext = '.res'
    out = self.make_bld_node('obj', node.parent, node.name[:node.name.rfind('.')] + obj_ext)
    rctask = self.create_task('winrc', node, out)
    try:
        self.compiled_tasks.append(rctask)
    except AttributeError:
        self.compiled_tasks = [rctask]


@extension('.c', '.m')
def c_hook(self, node):
    if 'master_folder' in self.features and not Options.options.nomaster:
        try:
            mastertask_c = self.mastertasks_c_folders[node.parent]
            mastertask_c.set_inputs([node])
        except AttributeError:
            output = self.make_bld_node('src', None, 'master-c-%s-0.%s' % (node.parent.name, self.objc and 'm' or 'c'))
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
    elif 'master' in self.features and not Options.options.nomaster:
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


@extension('.cc', '.cxx', '.cpp', '.mm')
def cc_hook(self, node):
    if 'master_folder' in self.features and not Options.options.nomaster:
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
                'src', None,
                'master-c-%s-%d.%s' % (node.parent.name, len(self.mastertasks_cxx_folders), self.objc and 'mm' or 'cc')
            )
            mastertask_cxx = self.create_task('master', [node], [output])
            self.mastertasks_cxx_folders[node.parent] = mastertask_cxx
            self.create_compiled_task('cxx', output)
    elif 'master' in self.features and not Options.options.nomaster:
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


@feature('master', 'master_folder')
def dummy_hook_master(self):
    pass


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
    out_node = self.make_bld_node('bin', None, os.path.join(path, pattern % name))
    self.link_task.set_outputs(out_node)


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
def apply_implib(self):
    pass


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('apply_link')
@before_method('install_step')
def set_postlink_task(self):
    self.postlink_task = self.link_task


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('set_postlink_task')
def install_step(self):
    pass


@feature('c', 'cxx')
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


Task.task_factory('dbg_copy', '${OBJCOPY} --only-keep-debug ${SRC} ${TGT[0].abspath()}', color='BLUE')
Task.task_factory('dbg_strip', '${STRIP} ${STRIPFLAGS} -S -o ${TGT[0].abspath()} ${SRC[0].abspath()}', color='BLUE')
dbg_link_cls = Task.task_factory(
    'dbg_link',
    '${OBJCOPY} --add-gnu-debuglink=${SRC[0].path_from(tsk.inputs[1].parent)} ${SRC[1].abspath()} ${TGT[0].abspath()}',
    color='BLUE'
)
dbg_link_cls.exec_command = exec_command_objcopy


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
            stripped_prog = out_dir.make_node(full_link.name + '.stripped')
            stripped_linked_prog = out_dir.make_node(full_link.name)

            if 'plugin' in self.features:
                out_path = self.env.DEPLOY_PLUGINDIR
            elif 'kernel' in self.features:
                out_path = self.env.DEPLOY_KERNELDIR
            elif 'cshlib' in self.features or 'cxxshlib' in self.features:
                out_path = self.env.DEPLOY_RUNBINDIR
            elif 'cprogram' in self.features or 'cxxprogram' in self.features:
                out_path = self.env.DEPLOY_BINDIR
            else:
                return

            self.dbg_copy_task = self.create_task('dbg_copy', [full_link], [debug_prog])
            self.strip_task = self.create_task('dbg_strip', [full_link], [stripped_prog])
            self.dbg_link_task = self.create_task('dbg_link', [debug_prog, stripped_prog], [stripped_linked_prog])
            self.dbg_link_task.cwd = out_dir.abspath()
            self.postlink_task = self.dbg_link_task
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path), [debug_prog])


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@before_method('install_step')
@after_method('set_postlink_task')
def strip_debug_info(self):
    if not self.env.ENV_PREFIX:
        self.strip_debug_info_impl()
