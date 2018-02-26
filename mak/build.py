from waflib import Task, Options, Build, Task, Utils, Errors, TaskGen
from waflib.Configure import conf
from waflib.TaskGen import feature, taskgen_method, extension, before_method, after_method
import os
import re
import shutil
from waflib.Tools import ccroot, c, cxx, winres


old_log_display = Task.Task.log_display
old_exec_command = Task.Task.exec_command


def to_string(self):
    bld = self.generator.bld
    if self.outputs:
        tgt_str = ' '.join([a.name for a in self.outputs][:1])
    else:
        tgt_str = ' '.join([a.name for a in self.inputs][:1])
    task_name = self.__class__.__name__.replace('_task', '')
    return '{%s}%s%s/%s\n' % (task_name,
                              (11-len(task_name))*' ',
                              self.generator.target,
                              tgt_str)


def create_namespace_file(task):
    with open(task.outputs[0].abspath(), 'w') as f:
        namespaces = []
        for k_name, k in task.generator.kernels:
            full_ns = ['Kernels']
            full_ns += [n.capitalize() for n in k_name[:-1]]
            for i in range(1, len(full_ns)+1):
                ns = full_ns[0:i]
                if ns not in namespaces:
                    namespaces.append(ns)
        pch = getattr(task, 'pch', '')
        if pch:
            f.write('#include <%s>\n' % pch)
        f.write('#include <plugin/stdafx.h>\n')
        f.write('#include <rtti/engine/namespace.hh>\n')

        for n in namespaces:
            f.write('BE_REGISTER_NAMESPACE_%d(%s);\n'%(len(n), ', '.join(n)))
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


def exec_command(self, *k, **kw):
    if 'filter' not in kw:
        kw['filter'] = filter
    return old_exec_command(self, *k, **kw)


Task.Task.__str__ = to_string
Task.Task.log_display = log_display
Task.Task.exec_command = exec_command


class install_task(Task.Task):
    color = 'CYAN'

    def runnable_status(self):
        ret = super(install_task, self).runnable_status()
        if ret != Task.ASK_LATER:
            for source, target, chmod in self.install_step:
                d, _ = os.path.split(target)
                if not d:
                    raise Errors.WafError('Invalid installation given %r->%r' % (src, tgt))

                try:
                    st1 = os.stat(target)
                    st2 = os.stat(source.abspath())
                except OSError:
                    ret = Task.RUN_ME
                    break
                else:
                    # same size and identical timestamps -> make no copy
                    if st1.st_mtime + 2 < st2.st_mtime or st1.st_size != st2.st_size:
                        ret = Task.RUN_ME
                        break
        return ret

    def run(self):
        for source, target, chmod in self.install_step:
            d, _ = os.path.split(target)
            Utils.check_dir(d)
            # following is for shared libs and stale inodes (-_-)
            try:
                os.remove(target)
            except OSError:
                pass

            try:
                shutil.copy2(source.abspath(), target)
                os.chmod(target, chmod)
            except IOError:
                try:
                    os.stat(source.abspath())
                except (OSError, IOError):
                    Logs.error('File %r does not exist' % source.abspath())
                    return 1
                Logs.error('Could not install the file %r' % target)
        return 0


for command in ['build', 'clean']:
    for variant in ['debug', 'profile', 'final']:
        class BuildWrapperVariant(Build.BuildContext):
            cmd = '%s:all:%s'%(command, variant)
            def execute(self):
                self.restore()
                if not self.all_envs:
                    self.load_envs()
                for toolchain in self.env.ALL_TOOLCHAINS:
                    cmd, all, variant = self.cmd.split(':')
                    Options.commands.append('%s:%s:%s'%(cmd, toolchain, variant))
    class BuildWrapperAll(Build.BuildContext):
        cmd = '%s:all'%command
        def execute(self):
            self.restore()
            if not self.all_envs:
                self.load_envs()
            for toolchain in self.env.ALL_TOOLCHAINS:
                for variant in self.env.ALL_VARIANTS:
                    Options.commands.append('%s:%s:%s'%(self.cmd[:-4], toolchain, variant))


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
def add_feature(self, feature, env = None):
    if env:
        env.append_unique('FEATURES', feature)
    else:
        for env in self.multiarch_envs:
            env.append_unique('FEATURES', feature)


@conf
def module(bld, name, module_path, depends, private_depends,
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
    project_path = source_node.path_from(bld.srcnode).replace('/', '.')
    if 'plugin' in features:
        plugin_name = name.replace('.', '_')
    else:
        plugin_name = 'bugengine'

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
    static_defines = bld.env.STATIC and ['BE_STATIC=1'] or []
    extra_defines = extra_defines + static_defines
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
    include = [i for i in [source_node.make_node('include')] if os.path.isdir(i.abspath())] + [bld.bldnode]
    lib_paths = [i.path_from(bld.bldnode) for i in [source_node.make_node('lib')] if os.path.isdir(i.abspath())]

    if api and os.path.isfile(os.path.join(api[0].abspath(), module_path.split('.')[-1], 'stdafx.h')):
        pchstop = '%s/%s'%(module_path.split('.')[-1], 'stdafx.h')
    elif include and os.path.isfile(os.path.join(include[0].abspath(), 'stdafx.h')):
        pchstop = 'stdafx.h'
    else:
        pchstop = None

    if use_master == 'folder':
        features = features + ['master_folder']
    elif use_master == True:
        features = features + ['master']
    elif use_master != False:
        raise Errors.WafError('unknown value for use_master: %s' % use_master)
    if warnings:
        extra_features = ['warnall', bld.__class__.optim] + (bld.env.STATIC and [] or ['dynamic'])
    else:
        extra_features = ['warnnone', bld.__class__.optim] + (bld.env.STATIC and [] or ['dynamic'])

    result = []
    internal_deps = {}

    if build and not bld.env.PROJECTS:
        preprocess = bld(
            env=bld.common_env.derive(),
            target = name + '.preprocess',
            parent = name,
            features= ['preprocess'],
            pchstop = pchstop,
            source = preprocess_sources,
            kernels = [])
        preprocess.env.PLUGIN = plugin_name
        if os.path.isdir(os.path.join(source_node.abspath(), 'kernels')):
            kernelspath = source_node.make_node('kernels')
            for kernel in kernelspath.ant_glob('**'):
                kernel_name = os.path.splitext(kernel.path_from(kernelspath))[0]
                kernel_name = re.split('[\\\\/]', kernel_name)
                preprocess.kernels.append((kernel_name, kernel))
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
            bld_env = env,
            target = target_prefix + name,
            target_name = name,
            module_path = project_path,
            use = [target_prefix + d for d in depends],
            private_use = [target_prefix + d for d in private_depends],
            features = features,
            extra_use = extra_features,
            defines = ['building_%s' % safe_name(name.split('.')[-1]),
                       'BE_PROJECTID=%s'%name.replace('.', '_'),
                       'BE_PROJECTNAME=%s'%name] + extra_defines,
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
        result.append(task_gen)
        if target_prefix:
            try:
                internal_deps[name].append(target_prefix + name)
            except KeyError:
                internal_deps[name] = [target_prefix + name]
        for kernel_type, toolchain in env.KERNEL_TOOLCHAINS:
            kernels = preprocess and preprocess.kernels or []
            kernel_env = bld.all_envs[toolchain]
            for kernel, kernel_source in kernels:
                for variant in [''] + kernel_env.KERNEL_OPTIM_VARIANTS:
                    target_suffix = '.'.join([kernel_type] + ([variant[1:]] if variant else []))
                    kernel_target = name + '.' + '.'.join(kernel) + '.' + target_suffix
                    kernel_task_gen = bld(
                            env = env.derive(),
                            bld_env = env,
                            kernel_env = kernel_env,
                            target = target_prefix + kernel_target,
                            target_name = target_prefix + name,
                            kernel = kernel,
                            features = ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel'],
                            extra_use = task_gen.extra_use,
                            defines = task_gen.defines + [
                                    'BE_BUILD_KERNEL=1',
                                    'BE_KERNEL_ID=%s_%s'%(name.replace('.', '_'), kernel_target.replace('.', '_')),
                                    'BE_KERNEL_NAME=%s.%s'%(name, kernel_target),
                                    'BE_KERNEL_TARGET=%s' % kernel_type,
                                    'BE_KERNEL_ARCH=%s' % variant],
                            includes = task_gen.includes,
                            source = [kernel_source],
                            use = task_gen.use + ([variant] if variant else []),
                        )
                    kernel_task_gen.env.PLUGIN = plugin_name
                    if target_prefix:
                        try:
                            internal_deps[kernel_target].append(target_prefix + kernel_target)
                        except KeyError:
                            internal_deps[kernel_target] = [target_prefix + kernel_target]
    multiarch = None
    for multiarch_target, deps in internal_deps.items():
        tgt = bld(target=multiarch_target, features=['multiarch'], use=deps)
        if multiarch_target == name:
            multiarch = tgt

    if multiarch or result:
        install_tg = multiarch if multiarch else result[0]
        for p,e in [
                ('data', 'DEPLOY_DATADIR'),
                ('bin', 'DEPLOY_RUNBINDIR')]:
            node = source_node.make_node(p)
            if os.path.isdir(node.abspath()):
                install_tg.deploy_directory(bld.env, node, '', e)
            for tp in platforms:
                node = source_node.make_node(p+'.'+tp)
                if os.path.isdir(node.abspath()):
                    install_tg.deploy_directory(bld.env, node, '', e)
                for a in archs:
                    node = source_node.make_node(p+'.'+tp+'.'+a)
                    if os.path.isdir(node.abspath()):
                        install_tg.deploy_directory(bld.env, node, '', e)

    return (result, multiarch)


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
def thirdparty(bld, name, feature='', path='.', var='', use=[], private_use=[]):
    platforms = bld.env.VALID_PLATFORMS
    platform_specific = platforms
    source_node = bld.path.make_node(path.replace('.', '/'))
    project_path = source_node.parent.path_from(bld.srcnode).replace('/', '.')
    project_path = '%s.%s' % (project_path, name.split('.')[-1])
    if not var: var = bld.path.name

    internal_deps = []
    supported = False
    for env in bld.multiarch_envs:
        target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
        target_name = target_prefix + name
        if env['check_%s' % var] or env.PROJECTS:
            if feature:
                bld.add_feature(feature, env)
            supported = True
            tg = bld(target             = target_name,
                     features           =['cxx'],
                     module_path        = project_path,
                     export_includes    = env['check_%s_includes' % var],
                     export_defines     = env['check_%s_defines' % var],
                     export_libpath     = env['check_%s_libpath' % var],
                     export_lib         = env['check_%s_libs' % var],
                     export_framework   = env['check_%s_frameworks' % var],
                     export_cflags      = env['check_%s_cflags' % var],
                     export_cxxflags    = env['check_%s_cxxflags' % var],
                     export_linkflags   = env['check_%s_ldflags' % var],
                     source_nodes       = [source_node],
                     use                = [target_prefix+u for u in use],
                     private_use        = [target_prefix+u for u in private_use])
            if target_prefix:
                internal_deps.append(tg)
            archs = env.VALID_ARCHITECTURES
            arch_specific = archs + ['%s.%s'%(p,a) for p in platforms for a in archs]
            bin_paths = [i for i in [source_node.make_node('bin.%s'%arch) for arch in arch_specific] if os.path.isdir(i.abspath())]
            data_paths = [i for i in [source_node.make_node('data.%s'%arch) for arch in arch_specific] if os.path.isdir(i.abspath())]
            for bin_path in bin_paths:
                tg.deploy_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
            for data_path in data_paths:
                tg.deploy_directory(env, data_path, '', 'DEPLOY_DATADIR')
            if name not in bld.env.THIRDPARTIES_FIRST:
                bin_paths = [i for i in [source_node.make_node('bin')] + [source_node.make_node('bin.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]
                for bin_path in bin_paths:
                    tg.deploy_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
                bld.env.append_unique('THIRDPARTIES_FIRST', name)

    if supported:
        if internal_deps:
            tg = bld(target=name, features=['multiarch'], use=[d.name for d in internal_deps])

        if name not in bld.env.THIRDPARTIES_FIRST:
            bin_paths = [i for i in [source_node.make_node('bin')] + [source_node.make_node('bin.%s'%platform) for platform in platform_specific] if os.path.isdir(i.abspath())]
            for bin_path in bin_paths:
                tg.deploy_directory(bld.env, bin_path, '', 'DEPLOY_RUNBINDIR')
            bld.env.append_unique('THIRDPARTIES_FIRST', name)
    return supported


@conf
def library(bld, name, depends=[], private_use=[], features=[], platforms=[],
            extra_includes=[], extra_defines=[],
            extra_public_includes=[], extra_public_defines=[],
            extra_tasks=[],
            path='', use_master=True, warnings=True, export_all=False):
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + (bld.env.DYNAMIC and ['cxx', 'cxxshlib', 'shared_lib'] or ['cxx', 'cxxobjects']),
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, export_all)


@conf
def headers(bld, name, depends=[], private_use=[], features=[], platforms=[],
            extra_public_includes=[], extra_public_defines=[],
            extra_tasks=[],
            path='', use_master=True, warnings=True, export_all=False):
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + ['cxx'],
        features,
        [], [],
        extra_public_includes, extra_public_defines,
        use_master, warnings, export_all)


@conf
def static_library(bld, name, depends=[], private_use=[], features=[], platforms=[],
                   extra_includes=[], extra_defines=[],
                   extra_public_includes=[], extra_public_defines=[],
                   extra_tasks=[],
                   path='', use_master=True, warnings=True):
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + ['cxx', 'cxxstlib'],
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, False)


@conf
def shared_library(bld, name, depends=[], private_use=[], features=[], platforms=[],
                   extra_includes=[], extra_defines=[],
                   extra_public_includes=[], extra_public_defines=[],
                   extra_tasks=[],
                   path='', use_master=True, warnings=True, export_all=False):
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + (bld.env.STATIC and ['cxx', 'cxxobjects'] or ['cxx', 'cxxshlib', 'shared_lib']),
        features,
        extra_includes, extra_defines,
        extra_public_includes, extra_public_defines,
        use_master, warnings, export_all)


@conf
def engine(bld, name, depends=[], private_use=[], features=[], platforms=[],
           extra_includes=[], extra_defines=[],
           extra_public_includes=[], extra_public_defines=[],
           extra_tasks=[],
           path='', use_master=True, warnings=True):
    if getattr(bld, 'launcher', None) != None:
        raise Errors.WafError('Only one engine can be defined')
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    bld.launcher = module(bld, name, path, depends + ['3rdparty.system.console'], private_use, platforms,
                          extra_tasks + ['cxx', 'cxxprogram', 'launcher'], features,
                          extra_includes, extra_defines, extra_public_includes, extra_public_defines,
                          use_master, warnings, False)
    if 'windows' in bld.env.VALID_PLATFORMS:
        module(bld, name+'w', path, depends, private_use, platforms, extra_tasks + ['cxx', 'cxxprogram', 'launcher'],
               features, extra_includes, extra_defines, extra_public_includes, extra_public_defines,
               use_master, warnings, False)


@conf
def game(bld, name, depends=[], private_use=[], features=[], platforms=[],
         extra_includes=[], extra_defines=[],
         extra_public_includes=[], extra_public_defines=[],
         extra_tasks=[],
         path='', use_master=True, warnings=True):
    if not path: path=name
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'plugin', 'game'],
        features, extra_includes, extra_defines, extra_public_includes, extra_public_defines,
        use_master, warnings, False)


@conf
def plugin(bld, name, depends=[], private_use=[], features=[], platforms=[],
           extra_includes=[], extra_defines=[],
           extra_public_includes=[], extra_public_defines=[],
           extra_tasks=[],
           path='', use_master=True, warnings=True):
    if not path: path=name
    for p in platforms:
        if p not in bld.env.VALID_PLATFORMS:
            return None
    return module(bld, name, path, depends, private_use, platforms,
        extra_tasks + ['cxx', bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'plugin'],
        features, extra_includes, extra_defines, extra_public_includes, extra_public_defines,
        use_master, warnings, False)


def build(bld):
    bld.load('cpp_parser', tooldir=[os.path.join(bld.path.abspath(), 'tools', 'waf')])
    bld.load('data', tooldir=[os.path.join(bld.path.abspath(), 'tools', 'waf')])
    bld.load('kernel_task', tooldir=[os.path.join(bld.path.abspath(), 'tools','waf')])
    bld.load('clc', tooldir=[os.path.join(bld.path.abspath(), 'tools', 'waf')])
    bld.load('kernel_cpu', tooldir=[os.path.join(bld.path.abspath(), 'tools', 'waf')])
    bld.env.STATIC = bld.env.STATIC or Options.options.static
    bld.env.DYNAMIC = Options.options.dynamic
    if bld.env.STATIC and bld.env.DYNAMIC:
        raise Errors.WafError('Engine requested to be built both as static and dynamic')
    bld.original_env = bld.env
    bld.multiarch_envs = [bld.all_envs[envname] for envname in bld.env.SUB_TOOLCHAINS] or [bld.env]


@taskgen_method
def deploy_directory(self, env, node, local_path, env_variable):
    target_path = os.path.join(self.bld.env.PREFIX, self.bld.__class__.optim,
                               env[env_variable], local_path)
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
                if ('plugin' in task_gen.features) and 'cxx' in task_gen.features:
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
        self.env.append_unique('CPPFLAGS', self.env['CPPFLAGS_%s'%f])
        self.env.append_unique('CFLAGS', self.env['CFLAGS_%s'%f])
        self.env.append_unique('CXXFLAGS', self.env['CXXFLAGS_%s'%f])
        self.env.append_unique('LINKFLAGS', self.env['LINKFLAGS_%s'%f])


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
    dependencies = [self.bld.get_tgen_by_name(i) for i in getattr(self, 'use', [])
                                                        + getattr(self, 'private_use', [])]
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
                    try: all_deps.remove((d, link_objects))
                    except ValueError: pass
                    all_deps.append((d, link_objects))
                dependencies += [(i, link_objects) for i in new_deps]
        for d, link_objects in all_deps:
            for var in 'LIB', 'LIBPATH', 'STLIB', 'STLIBPATH','LINKFLAGS', 'FRAMEWORK':
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
def make_bld_node_common(self, node, path, name):
    if not path:
        node = node.make_node(name)
    elif path.is_child_of(self.bld.bldnode):
        out_dir = path.path_from(self.bld.bldnode)
        # skip variant
        out_dir = out_dir[out_dir.find(os.path.sep)+1:]
        # skip target
        out_dir = out_dir[out_dir.find(os.path.sep)+1:]
        # skip category
        out_dir = out_dir[out_dir.find(os.path.sep)+1:]
        node = node.make_node(out_dir)
        node = node.make_node(name)
    else:
        out_dir = path.path_from(self.path)
        while out_dir[0] == '.':
            out_dir = out_dir[out_dir.find(os.path.sep)+1:]
        node = node.make_node(out_dir)
        node = node.make_node(name)
    node.parent.mkdir()
    return node


@taskgen_method
def make_bld_node(self, category, path, name):
    try:
        node = self.bld.bldnode.make_node(self.bld.optim).make_node(self.target).make_node(category)
    except AttributeError:
        node = self.bld.bldnode.make_node('_any_').make_node(self.target).make_node(category)
    return self.make_bld_node_common(node, path, name)


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
    out = self.make_bld_node('obj', node.parent, node.name[:node.name.rfind('.')]+'.o')
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
        out = self.make_bld_node('src', None, 'namespace.cc')
        self.create_task('namespace', [], [out])
        try:
            self.out_sources.append(out)
        except:
            self.out_sources = [out]


@extension('.rc')
def rc_file(self, node):
    """
    Bind the .rc extension to a winrc task
    """
    obj_ext = '.o'
    if self.env['WINRC_TGT_F'] == '/fo':
        obj_ext = '.res'
    out = self.make_bld_node('obj', node.parent, node.name[:node.name.rfind('.')]+obj_ext)
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
            self.mastertasks_c_folders = { node.parent: mastertask_c }
            self.create_compiled_task('c', output)
        except KeyError:
            output = self.make_bld_node('src', None, 'master-c-%s-%d.%s' % (node.parent.name, len(self.mastertasks_c_folders), self.objc and 'm' or 'c'))
            mastertask_c = self.create_task('master', [node], [output])
            self.mastertasks_c_folders[node.parent] = mastertask_c
            self.create_compiled_task('c', output)
    elif 'master' in self.features and not Options.options.nomaster:
        try:
            mastertask_c = self.mastertasks_c[-1]
            if len(mastertask_c.inputs) <= 10:
                mastertask_c.set_inputs([node])
            else:
                output = self.make_bld_node('src', None, 'master-c-%d.%s' % (len(self.mastertasks_c), self.objc and 'm' or 'c'))
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
            output = self.make_bld_node('src', None, 'master-c-%s-0.%s' % (node.parent.name, self.objc and 'mm' or 'cc'))
            mastertask_cxx = self.create_task('master', [node], [output])
            self.mastertasks_cxx_folders = { node.parent: mastertask_cxx }
            self.create_compiled_task('cxx', output)
        except KeyError:
            output = self.make_bld_node('src', None, 'master-c-%s-%d.%s' % (node.parent.name, len(self.mastertasks_cxx_folders), self.objc and 'mm' or 'cc'))
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
                    output = self.make_bld_node('src', None, 'master-cxx-%d.%s' % (len(self.mastertasks_cxx), self.objc and 'mm' or 'cc'))
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
    pattern = self.env['%s_PATTERN'%link]
    if not pattern:
        pattern = '%s'
    path, name = os.path.split(self.target)
    out_node = self.make_bld_node('bin', None, os.path.join(path, pattern%name))
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


Task.task_factory('dbg_copy',
                  '${OBJCOPY} --only-keep-debug ${SRC} ${TGT[0].abspath()}',
                  color='BLUE')
Task.task_factory('dbg_strip',
                  '${STRIP} ${STRIPFLAGS} -S -o ${TGT[0].abspath()} ${SRC[0].abspath()}',
                  color='BLUE')
dbg_link_cls = Task.task_factory('dbg_link',
                        '${OBJCOPY} --add-gnu-debuglink=${SRC[0].path_from(tsk.inputs[1].parent)} ${SRC[1].abspath()} ${TGT[0].abspath()}',
                        color='BLUE')
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
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path),
                               [debug_prog])


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@before_method('install_step')
@after_method('set_postlink_task')
def strip_debug_info(self):
    if not self.env.ENV_PREFIX:
        self.strip_debug_info_impl()
