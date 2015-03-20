from waflib import Task, Options, Build, Task, Utils, Errors, TaskGen
from waflib.Configure import conf
from waflib.TaskGen import feature, taskgen_method, extension, before_method, after_method
import os
from waflib.Tools import ccroot, c, cxx


old_log_display = Task.Task.log_display
old_exec_command = Task.Task.exec_command


def to_string(self):
    bld = self.generator.bld
    tgt_str = ' '.join([a.path_from(bld.bldnode) for a in self.outputs][:1])
    return '(%s) %s\n' % (self.__class__.__name__.replace('_task', ''), tgt_str)


def create_namespace_file(task):
    with open(task.outputs[0].abspath(), 'w') as f:
        pch = getattr(task, 'pch', '')
        if pch:
            f.write('#include <%s>\n' % pch)
        f.write('#include <plugin/stdafx.h>\n')
        f.write('#include <rtti/engine/namespace.hh>\n')
        namespace = [i.capitalize() for i in task.generator.target.split('.')[:-1]]
        for i in range(1, len(namespace)+1):
            ns = namespace[0:i]
            f.write('BE_REGISTER_NAMESPACE_%d(%s);\n'%(len(ns), ', '.join(ns)))
        f.write('BE_REGISTER_NAMESPACE_1(Kernels);\n')
NamespaceTask = Task.task_factory("namespace", create_namespace_file, color='CYAN')


def log_display(self, bld):
    if not Options.options.silent:
        old_log_display(self, bld)


def filter(exe, lines):
    result = []
    for line in lines:
        i1 = line.find('skipping incompatible ')
        i2 = line.find('when searching for')
        if i1 == -1 or i2 == -1:
            result.append(line)
    return result


def exec_command(self, cmd, **kw):
    kw['filter'] = filter
    return old_exec_command(self, cmd, **kw)


Task.Task.__str__ = to_string
Task.Task.log_display = log_display
Task.Task.exec_command = exec_command


for command in ['build', 'clean', 'install']:
    class BuildWrapperAll(Build.BuildContext):
        cmd = '%s:all'%command
        def execute(self):
            self.restore()
            if not self.all_envs:
                self.load_envs()
            for toolchain in self.env.ALL_TOOLCHAINS:
                for variant in self.env.ALL_VARIANTS:
                    Options.commands.append('%s:%s:%s'%(command, toolchain, variant))


def safe_name(name):
    name = name.replace('-', '_')
    name = name.replace('+', 'x')
    return name


def options(opt):
    opt.add_option('--profile', action='store_true', default=False, dest='profile', help='run WAF in the profiler')
    gr = opt.get_option_group('build and install options')
    gr.add_option('--nomaster',
                  action = 'store_true',
                  default = False,
                  dest = 'nomaster',
                  help = 'build without master files')
    gr.add_option('--static',
                  action = 'store_true',
                  default = False,
                  dest = 'static',
                  help = 'build completely static executable: All engine components, plugins, samples and kernels will be built into the executable')
    gr.add_option('--dynamic',
                  action = 'store_true',
                  default = False,
                  dest = 'dynamic',
                  help = 'build completely dynamic executable: All engine components, plugins, samples and kernels will be built as shared objects')
    gr.add_option('--silent',
                  action = 'store_true',
                  default = False,
                  dest = 'silent',
                  help = 'do not print build log from Waf')


@conf
def module(bld, name, module_path, depends,
           valid_platforms, features,
           build_features,
           extra_includes, extra_defines,
           extra_public_includes, extra_public_defines,
           use_master, warnings, export_all):
    platforms = bld.env.VALID_PLATFORMS
    archs = bld.env.ARCHITECTURES
    build = len(valid_platforms) == 0
    for p in valid_platforms:
        if p in platforms:
            build = True

    source_node = bld.path.make_node(module_path.replace('.', '/'))
    for p,e in [
            ('data', 'DEPLOY_DATADIR'),
            ('bin', 'DEPLOY_RUNBINDIR')]:
        node = source_node.make_node(p)
        if os.path.isdir(node.abspath()):
            deploy_directory(bld, bld.env, node, '', e)
        for tp in platforms:
            node = source_node.make_node(p+'.'+tp)
            if os.path.isdir(node.abspath()):
                deploy_directory(bld, bld.env, node, '', e)
            for a in archs:
                node = source_node.make_node(p+'.'+tp+'.'+a)
                if os.path.isdir(node.abspath()):
                    deploy_directory(bld, bld.env, node, '', e)

    if 'plugin' in features:
        plugin_name = name.replace('.', '_')
    else:
        plugin_name = 'game'

    compile_extensions = ['cxx', 'cpp', 'cc', 'c', 'rc', 'm', 'mm']
    preprocess_extensions = ['yy', 'll', 'plist']
    try:
        sources = source_node.ant_glob(['src/**/*.%s'%(ext) for ext in compile_extensions])
    except OSError:
        sources = []
    try:
        preprocess_sources = source_node.ant_glob(['src/**/*.%s'%(ext) for ext in preprocess_extensions])
    except OSError:
        preprocess_sources = []
    try:
        preprocess_sources += source_node.ant_glob(['api/**/*.script.hh'])
    except OSError:
        pass
    try:
        preprocess_sources += source_node.ant_glob(['include/**/*.script.hh'])
    except OSError:
        pass

    extras = []
    if bld.env.STATIC:
        extra_defines = extra_defines + ['BE_STATIC=1']
    if source_node.is_child_of(bld.bugenginenode):
        relative_path = source_node.path_from(bld.bugenginenode)
        for platform in bld.bugenginenode.make_node('extra').listdir():
            path = os.path.join('extra', platform, relative_path)
            extra_source_node = bld.bugenginenode.find_node(path)
            if extra_source_node:
                extras.append((platform, extra_source_node))
                source_filter = ['src/**/*.%s'%ext for ext in compile_extensions]
                sources += extra_source_node.ant_glob(source_filter)
                preprocess_sources += extra_source_node.ant_glob(['src/**/*'], excl=source_filter)

    api = [i for i in [source_node.make_node('api')] if os.path.isdir(i.abspath())]
    include = [i for i in [source_node.make_node('include')] if os.path.isdir(i.abspath())]
    lib_paths = [i.path_from(bld.bldnode) for i in [source_node.make_node('lib')] if os.path.isdir(i.abspath())]

    if api and os.path.isfile(os.path.join(api[0].abspath(), module_path.split('.')[-1], 'stdafx.h')):
        pchstop = '%s/%s'%(module_path.split('.')[-1], 'stdafx.h')
    elif include and os.path.isfile(os.path.join(include[0].abspath(), 'stdafx.h')):
        pchstop = 'stdafx.h'
    else:
        pchstop = None

    if use_master:
        features = features + ['master']
    if warnings:
        extra_features = ['warnall', bld.__class__.optim]
    else:
        extra_features = ['warnnone', bld.__class__.optim]

    result = []
    internal_deps = []
    internal_kernels = []

    if build and not bld.env.PROJECTS:
        preprocess = bld(
            env=bld.env.derive(),
            target = name + '.preprocess',
            features= ['preprocess'],
            pchstop = pchstop,
            source = preprocess_sources,
            kernels = [])
        preprocess.env.PLUGIN = plugin_name
        if os.path.isdir(os.path.join(source_node.abspath(), 'kernels')):
            kernelspath = source_node.make_node('kernels')
            for kernel in kernelspath.listdir():
                kernelpath = kernelspath.make_node(kernel)
                kernelsources = [kernelpath]
                kernels = []
                for env in bld.multiarch_envs:
                    target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
                    kernel_name = kernel[:kernel.rfind('.')]
                    t = bld(
                        env = env.derive(),
                        target = target_prefix + name + '.' + kernel_name,
                        features = ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel'],
                        extra_use = extra_features,
                        defines = [
                                'BE_BUILD_KERNEL=1',
                                'BE_KERNELID=%s_%s'%(name.replace('.', '_'),kernel_name),
                                'BE_KERNELNAME=%s.%s'%(name, kernel_name)],
                        includes = api + include,
                        kernel_source = kernelsources,
                        use = [target_prefix + d for d in depends] + [target_prefix + name])
                    t.env.PLUGIN = plugin_name
                    kernels.append(target_prefix + name + '.' + kernel_name)
                if target_prefix:
                    internal_kernels.append((kernel_name, kernels, kernelsources))
                preprocess.kernels.append((kernel_name, kernelsources))
    else:
        preprocess = None

    for env in bld.multiarch_envs:
        exit = False
        if not env.PROJECTS:
            for feature in build_features:
                if feature not in env.FEATURES:
                    exit = True
        if exit:
            continue
        archs = env.VALID_ARCHITECTURES
        platform_specific = archs + platforms + ['%s.%s'%(p,a) for p in platforms for a in archs]

        platform_api = [i for i in [source_node.make_node('api.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]
        platform_include = [i for i in [source_node.make_node('include.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]
        platform_lib_paths = [i for i in [source_node.make_node('lib.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]

        for platform, extra_source_node in extras:
            if platform in platforms:
                n = extra_source_node.make_node('api')
                if os.path.isdir(n.abspath()):
                    api += [n]
                n = extra_source_node.make_node('include')
                if os.path.isdir(n.abspath()):
                    include += [n]

        target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''

        if not build:
            sources = []
            if 'cxxshlib' in features:
                features.remove('cxxshlib')
            if 'cxxprogram' in features:
                features.remove('cxxprogram')
            if 'cxxstlib' in features:
                features.remove('cxxstlib')
        task_gen = bld(
            env = env.derive(),
            target = target_prefix + name,
            target_name = name,
            use = [target_prefix + d for d in depends],
            features = features,
            extra_use = extra_features,
            defines = ['building_%s' % safe_name(name.split('.')[-1]), 'BE_PROJECTID=%s'%name.replace('.', '_'), 'BE_PROJECTNAME=%s'%name] + extra_defines,
            export_defines = [] + extra_public_defines,
            includes = extra_includes + api + platform_api + include + platform_include + [bld.bugenginenode],
            libs = [],
            lib_paths = lib_paths,
            export_includes = api + platform_api + extra_public_includes,
            frameworks = [],
            source = sources[:],
            pchstop = pchstop,
            preprocess = preprocess,
            export_all=export_all,
            source_nodes = [source_node] + [e for _, e in extras])
        task_gen.env.PLUGIN = plugin_name
        result.append(task_gen)
        if target_prefix:
            internal_deps.append(target_prefix + name)
    for kernel, kernel_deps, kernel_sources in internal_kernels:
        if kernel_deps:
            bld(target=name + '.' + kernel, features=['multiarch'], use=kernel_deps)
    if internal_deps:
        multiarch = bld(target=name, features=['multiarch'], use=internal_deps)
    else:
        multiarch = None
    return (result, multiarch)


def deploy_directory(bld, env, node, local_path, env_variable):
    target_path = os.path.join(bld.env.PREFIX, bld.__class__.optim, env[env_variable], local_path)
    for n in node.listdir():
        sn = node.make_node(n)
        if os.path.isdir(sn.abspath()):
            deploy_directory(bld, env, sn, os.path.join(local_path, n), env_variable)
        else:
            bld.install_as(os.path.join(target_path, n), sn)


@conf
def external(bld, name):
    namespace = name.split('.')
    script_file = os.path.join( '/'.join(namespace), '%s_build.py' % namespace[-1])
    if os.path.isfile(os.path.join(bld.path.abspath(), script_file)):
        bld.recurse(script_file)
    else:
        bld.recurse(os.path.join(bld.bugenginenode.abspath(), 'src',  script_file))
    return name


@conf
def thirdparty(bld, name, path, env, libs=[], lib_paths=[], frameworks=[], includes=[], defines=[]):
    archs = env.VALID_ARCHITECTURES
    platforms = env.VALID_PLATFORMS
    platform_specific = platforms
    arch_specific = archs + ['%s.%s'%(p,a) for p in platforms for a in archs]
    source_node = bld.path.make_node('%s' % path.replace('.', '/'))

    lib_paths += [i.path_from(bld.bldnode) for i in [source_node.make_node('lib')] + [source_node.make_node('lib.%s'%platform) for platform in platform_specific + arch_specific] if os.path.isdir(i.abspath())]
    bin_paths = [i for i in [source_node.make_node('bin.%s'%arch) for arch in arch_specific] if os.path.isdir(i.abspath())]
    data_paths = [i for i in [source_node.make_node('data.%s'%arch) for arch in arch_specific] if os.path.isdir(i.abspath())]
    includes += [i for i in [source_node.make_node('api')] + [source_node.make_node('api.%s'%platform) for platform in platform_specific + arch_specific] if os.path.isdir(i.abspath())]

    target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
    target_name = target_prefix + name
    env['DEFINES_%s' % target_name] = defines
    env['INCLUDES_%s' % target_name] = includes
    env['FRAMEWORK_%s' % target_name] = frameworks
    env['LIBPATH_%s' % target_name] = lib_paths
    env['LIB_%s' % target_name] = libs
    for bin_path in bin_paths:
        deploy_directory(bld, bld.env, bin_path, '', 'DEPLOY_RUNBINDIR')
    for data_path in data_paths:
        deploy_directory(bld, bld.env, data_path, '', 'DEPLOY_DATADIR')
    if name not in bld.env.THIRDPARTIES_FIRST:
        bin_paths = [i for i in [source_node.make_node('bin')] + [source_node.make_node('bin.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]
        for bin_path in bin_paths:
            deploy_directory(bld, bld.env, bin_path, '', 'DEPLOY_RUNBINDIR')
        bld.env.append_unique('THIRDPARTIES_FIRST', name)
    if bld.env.PROJECTS:
        bld(target=name,
            includes=includes,
            defines=defines,
            source_nodes=[source_node])


@conf
def library(bld, name, depends=[], features=[], platforms=[],
        extra_includes=[], extra_defines=[],
        extra_public_includes=[], extra_public_defines=[],
        path='', use_master=True, warnings=True, export_all=False):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    module(bld, name, path, depends, platforms,
        ['cxx', 'cxxobjects'],
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, export_all)


@conf
def static_library(bld, name, depends=[], features=[], platforms=[],
        extra_includes=[], extra_defines=[],
        extra_public_includes=[], extra_public_defines=[],
        path='', use_master=True, warnings=True):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    module(bld, name, path, depends, platforms,
        ['cxx', 'cxxstlib'],
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, False)


@conf
def shared_library(bld, name, depends=[], features=[], platforms=[],
        extra_includes=[], extra_defines=[],
        extra_public_includes=[], extra_public_defines=[],
        path='', use_master=True, warnings=True, export_all=False):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    module(bld, name, path, depends, platforms,
        bld.env.STATIC and ['cxx', 'cxxobjects'] or ['cxx', 'cxxshlib', 'shared_lib'],
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, export_all)


@conf
def engine(bld, name, depends=[], features=[], platforms=[], path='', use_master=True, warnings=True):
    if getattr(bld, 'launcher', None) != None:
        raise Errors.WafError('Only one engine can be defined')
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    bld.launcher = module(bld, name, path, depends + ['3rdparty.console'], platforms, ['cxx', 'cxxprogram', 'launcher'],
                          features, [], [], [], [], use_master, warnings, False)
    if 'windows' in bld.env.VALID_PLATFORMS:
        module(bld, name+'w', path, depends, platforms, ['cxx', 'cxxprogram', 'launcher'],
               features, [], [], [], [], use_master, warnings, False)


@conf
def game(bld, name, depends=[], features=[], platforms=[], path='', use_master=True, warnings=True):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    module(bld, name, path, depends, platforms,
        ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'plugin', 'game'],
        features, [], [], [], [], use_master, warnings, False)


@conf
def plugin(bld, name, depends=[], features=[], platforms=[], path='', use_master=True, warnings=True):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    module(bld, name, path, depends, platforms,
        ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'plugin'],
        features, [], [], [], [], use_master, warnings, False)


def build(bld):
    bld.load('data', tooldir=[os.path.join(bld.path.abspath(), 'tools')])
    bld.load('kernel', tooldir=[os.path.join(bld.path.abspath(), 'tools')])
    bld.load('kernel_preprocess', tooldir=[os.path.join(bld.path.abspath(), 'tools')])
    bld.env.STATIC = bld.env.STATIC or Options.options.static
    bld.original_env = bld.env
    bld.multiarch_envs = [bld.all_envs[envname] for envname in bld.env.SUB_TOOLCHAINS] or [bld.env]


@feature('*')
def check_use_taskgens(self):
    """
    Checks all names in 'use' are valid task generators, or move them to uselib
    """
    use = getattr(self, 'use', [])
    for name in use:
        try:
            y = self.bld.get_tgen_by_name(name)
        except:
            try:
                self.uselib.append(name)
            except AttributeError:
                self.uselib = [name]


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
                self.env.append_unique('DEFINES', 'building_%s' % y.target.split('.')[-1])


@feature('launcher')
@before_method('apply_link')
@before_method('process_use')
def static_dependencies(self):
    if self.bld.env.STATIC:
        for g in self.bld.groups:
            for task_gen in g:
                if not isinstance(task_gen, TaskGen.task_gen):
                    continue
                if ('plugin' in task_gen.features or 'kernel' in task_gen.features) and 'cxx' in task_gen.features:
                    if task_gen.env.TOOLCHAIN == self.env.TOOLCHAIN:
                        self.use.append(task_gen.target)


@feature('launcher_static')
@before_method('apply_link')
def rename_executable(self):
    self.target = self.real_target


@feature('kernel')
@after_method('apply_link')
def install_kernel(self):
    if self.bld.is_install:
        if not self.env.ENV_PREFIX and not self.bld.env.STATIC: #no multiarch, no static
            self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_KERNELDIR), [self.link_task.outputs[0]])
            if self.env.CC_NAME == 'msvc':
                self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_KERNELDIR), [self.link_task.outputs[1]])


@feature('plugin')
@after_method('apply_link')
def install_plugin(self):
    if self.bld.is_install:
        if ('cshlib' in self.features) or ('cxxshlib' in self.features):
            if not self.env.ENV_PREFIX: #no multiarch
                self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_PLUGINDIR), [self.link_task.outputs[0]])
                if self.env.CC_NAME == 'msvc':
                    self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_PLUGINDIR), [self.link_task.outputs[1]])


@feature('shared_lib')
@after_method('apply_link')
def install_shared_lib(self):
    if self.bld.is_install:
        if ('cshlib' in self.features) or ('cxxshlib' in self.features):
            if not self.env.ENV_PREFIX: #no multiarch
                self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR), [self.link_task.outputs[0]])
                if self.env.CC_NAME == 'msvc':
                    self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR), [self.link_task.outputs[1]])


@feature('launcher')
@after_method('apply_link')
def install_program(self):
    if self.bld.is_install:
        if not self.env.ENV_PREFIX: #no multiarch
            self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_BINDIR), [self.link_task.outputs[0]], chmod=Utils.O755)
            if self.env.CC_NAME == 'msvc':
                self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_BINDIR), [self.link_task.outputs[1]])


@feature('game')
@after_method('apply_link')
def install_game(self):
    pass #also plugin


@feature('launcher')
def launcher_feature(task):
    pass


@feature('cxxobjects')
def objects_feature(task):
    pass


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
    for f in getattr(self, 'extra_use', []):
        self.env.append_unique('CFLAGS', self.env['CFLAGS_%s'%f])
        self.env.append_unique('CXXFLAGS', self.env['CXXFLAGS_%s'%f])
        self.env.append_unique('LINKFLAGS', self.env['LINKFLAGS_%s'%f])


@feature('cxx')
@before_method('filter_sources')
def gather_extra_source(self):
    preprocess = getattr(self, 'preprocess', None)
    if preprocess:
        preprocess.post()
        self.source += getattr(preprocess, 'out_sources', [])

@taskgen_method
def make_bld_node(self, category, path, name):
    if not path:
        node = self.bld.bldnode.make_node(category)
        node = node.make_node(self.target)
        node = node.make_node(name)
    elif path.is_child_of(self.bld.bldnode):
        out_dir = path.path_from(self.bld.bldnode)
        if out_dir[0] == '.':
            out_dir = os.path.join(category, out_dir[out_dir.find(os.path.sep)+1:])
        node = self.bld.bldnode.make_node(out_dir).make_node(name)
    else:
        node = self.bld.bldnode.make_node(category)
        node = node.make_node(self.target)
        node = node.make_node(path.path_from(self.path))
        node = node.make_node(name)
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
        basename,ext = os.path.splitext(file.name)
        add_platform = True
        add_arch = True
        platform = file.name.find('-p=')
        if platform != -1:
            add_platform = False
            platforms = basename[platform+3:].split(',')
            for p in platforms:
                add_platform = add_platform or p in self.env.VALID_PLATFORMS
        arch = file.name.find('-a=')
        if arch != -1:
            add_arch = False
            architectures = basename[arch+3:].split(',')
            for a in architectures:
                add_arch = add_arch or a in self.env.VALID_ARCHITECTURES
        node = file.parent
        while add_platform and add_arch and node and node != self.path and node != self.bld.srcnode:
            if node.name.startswith('platform='):
                add_platform = False
                platforms = node.name[9:].split(',')
                for p in platforms:
                    add_platform = add_platform or p in self.env.VALID_PLATFORMS
            elif node.name.startswith('arch='):
                add_arch = False
                architectures = node.name[5:].split(',')
                for a in architectures:
                    add_arch = add_arch or a in self.env.VALID_ARCHITECTURES
            elif node.parent.name == 'extra' and node.parent.parent == self.bld.bugenginenode:
                add_platform = node.name in self.env.VALID_PLATFORMS
            node = node.parent
        if add_platform and add_arch:
            sources.append(file)
            if ext in ['.m', '.mm']:
                self.objc = True
    self.source = sources


def create_master_file(task):
    with open(task.outputs[0].abspath(), 'w') as f:
        for src in task.inputs:
            f.write('#include "%s"\n' % src.path_from(task.generator.bld.bugenginenode).replace('\\','/'))


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
    out = self.make_bld_node('.obj', node.parent, node.name[:node.name.rfind('.')]+'.o')
    task = self.create_task(name, node, out)
    try:
        self.compiled_tasks.append(task)
    except AttributeError:
        self.compiled_tasks = [task]
    return task


@feature('preprocess')
@before_method('process_source')
def create_kernel_namespace(self):
    kernels = getattr(self, 'kernels', [])
    if kernels:
        out = self.make_bld_node('.src', None, 'namespace.cc')
        self.create_task('namespace', [], [out])
        try:
            self.out_sources.append(out)
        except:
            self.out_sources = [out]


@extension('.c', '.m')
def c_hook(self, node):
    if 'master' in self.features and not Options.options.nomaster:
        try:
            mastertask_c = self.mastertasks_c[-1]
            if len(mastertask_c.inputs) <= 20:
                mastertask_c.set_inputs([node])
            else:
                output = self.make_bld_node('.src', None, 'master-c-%d.%s' % (len(self.mastertasks_c), self.objc and 'm' or 'c'))
                mastertask_c = self.create_task('master', [node], [output])
                self.mastertasks_c.append(mastertask_c)
                self.create_compiled_task('c', output)
        except:
            output = self.make_bld_node('.src', None, 'master-c-0.%s' % (self.objc and 'm' or 'c'))
            mastertask_c = self.create_task('master', [node], [output])
            self.mastertasks_c = [mastertask_c]
            self.create_compiled_task('c', output)
    else:
        self.create_compiled_task('c', node)


@extension('.cc', '.cxx', '.cpp', '.mm')
def cc_hook(self, node):
    if 'master' in self.features and not Options.options.nomaster:
        if node.name.endswith('-instances.cc'):
            try:
                self.instancetask_cxx.set_inputs([node])
            except:
                output = self.make_bld_node('.src', None, 'instances-master-cxx.%s' % (self.objc and 'mm' or 'cc'))
                self.instancetask_cxx = self.create_task('master', [node], [output])
                self.create_compiled_task('cxx', output)
        else:
            try:
                mastertask_cxx = self.mastertasks_cxx[-1]
                if len(mastertask_cxx.inputs) <= 20:
                    mastertask_cxx.set_inputs([node])
                else:
                    output = self.make_bld_node('.src', None, 'master-cxx-%d.%s' % (len(self.mastertasks_cxx), self.objc and 'mm' or 'cc'))
                    mastertask_cxx = self.create_task('master', [node], [output])
                    self.mastertasks_cxx.append(mastertask_cxx)
                    self.create_compiled_task('cxx', output)
            except:
                output = self.make_bld_node('.src', None, 'master-cxx-0.%s' % (self.objc and 'mm' or 'cc'))
                mastertask_cxx = self.create_task('master', [node], [output])
                self.mastertasks_cxx = [mastertask_cxx]
                self.create_compiled_task('cxx', output)
    else:
        self.create_compiled_task('cxx', node)

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
    pattern = self.env['%s_PATTERN'%link]
    if not pattern:
        pattern = '%s'
    path, name = os.path.split(self.target)
    out_node = self.make_bld_node('.bin', None, os.path.join(path, pattern%name))
    self.link_task.set_outputs(out_node)

@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
def apply_implib(self):
    pass
