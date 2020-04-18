import os
import re
from waflib import Errors
from waflib.Configure import conf


def safe_name(name):
    name = name.replace('-', '_')
    name = name.replace('+', 'x')
    return name


@conf
def add_feature(self, feature, env=None):
    if env:
        env.append_unique('FEATURES', feature)
    else:
        for env in self.multiarch_envs:
            env.append_unique('FEATURES', feature)


@conf
def module(
    bld,
    name,
    module_path=None,
    depends=[],
    private_depends=[],
    valid_platforms=[],
    features=[],
    build_features=[],
    extra_includes=[],
    extra_defines=[],
    extra_public_includes=[],
    extra_public_defines=[],
    use_master=False,
    warnings=True,
    export_all=False,
    root_namespace=None
):
    platforms = bld.env.VALID_PLATFORMS
    archs = bld.env.ARCHITECTURES
    build = len(valid_platforms) == 0
    for p in valid_platforms:
        if p in platforms:
            build = True

    if module_path is None:
        source_node = bld.path.make_node(name.replace('.', '/'))
    else:
        source_node = module_path
    try:
        bld.recurse(os.path.join(source_node.path_from(bld.path), 'build.py'))
    except Errors.WafError:
        pass
    project_path = source_node.path_from(bld.srcnode).replace('/', '.').replace('\\', '.')
    if 'plugin' in features:
        plugin_name = name.replace('.', '_')
    else:
        plugin_name = 'bugengine'

    compile_extensions = ['cxx', 'cpp', 'cc', 'c', 'rc', 'm', 'mm', 'def']
    preprocess_extensions = ['yy', 'll', 'plist']
    try:
        sources = source_node.ant_glob(['src/**/*.%s' % (ext) for ext in compile_extensions])
    except OSError:
        sources = []
    try:
        preprocess_sources = source_node.ant_glob(['src/**/*.%s' % (ext) for ext in preprocess_extensions])
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
                source_filter = ['src/**/*.%s' % ext for ext in compile_extensions]
                sources += extra_source_node.ant_glob(source_filter)
                preprocess_sources += extra_source_node.ant_glob(['src/**/*'], excl=source_filter)

    api = [i for i in [source_node.make_node('api')] if os.path.isdir(i.abspath())]
    include = [i for i in [source_node.make_node('include')] if os.path.isdir(i.abspath())] + [bld.bldnode]
    lib_paths = [i.path_from(bld.bldnode) for i in [source_node.make_node('lib')] if os.path.isdir(i.abspath())]

    if api and os.path.isfile(os.path.join(api[0].abspath(), name.split('.')[-1], 'stdafx.h')):
        pchstop = '%s/%s' % (name.split('.')[-1], 'stdafx.h')
    elif include and os.path.isfile(os.path.join(include[0].abspath(), 'stdafx.h')):
        pchstop = 'stdafx.h'
    else:
        pchstop = None

    master_includes = []
    if use_master == 'folder':
        features = features + ['master_folder']
        master_includes.append(bld.bldnode)
    elif use_master is True:
        features = features + ['master']
        master_includes.append(bld.bldnode)
    elif use_master is not False:
        raise Errors.WafError('unknown value for use_master: %s' % use_master)
    if warnings:
        extra_features = ['warnall', bld.__class__.optim] + (bld.env.STATIC and [] or ['dynamic'])
    else:
        extra_features = ['warnnone', bld.__class__.optim] + (bld.env.STATIC and [] or ['dynamic'])

    result = []
    internal_deps = []

    if build and not bld.env.PROJECTS:
        preprocess = bld(
            env=bld.common_env.derive(),
            target=name + '.preprocess',
            parent=name,
            features=['preprocess'],
            pchstop=pchstop,
            source=preprocess_sources,
            kernels=[],
            source_nodes=[source_node] + [e for _, e in extras],
            root_namespace=root_namespace
        )
        preprocess.env.PLUGIN = plugin_name
        if os.path.isdir(os.path.join(source_node.abspath(), 'kernels')):
            kernelspath = source_node.make_node('kernels')
            for kernel in kernelspath.ant_glob('**'):
                kernel_name = os.path.splitext(kernel.path_from(kernelspath))[0]
                kernel_name = re.split('[\\\\/]', kernel_name)
                preprocess.kernels.append(
                    (
                        kernel_name, kernel,
                        preprocess.make_bld_node('src/kernels', None, '%s.ast' % (os.path.join(*kernel_name)))
                    )
                )
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
        platform_specific = archs + platforms + ['%s.%s' % (p, a) for p in platforms for a in archs]

        platform_api = [
            i for i in [source_node.make_node('api.%s' % platform) for platform in platform_specific]
            if os.path.isdir(i.abspath())
        ]
        platform_include = [
            i for i in [source_node.make_node('include.%s' % platform) for platform in platform_specific]
            if os.path.isdir(i.abspath())
        ]
        platform_lib_paths = [
            i for i in [source_node.make_node('lib.%s' % platform) for platform in platform_specific]
            if os.path.isdir(i.abspath())
        ]

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
            env=env.derive(),
            bld_env=env,
            target=target_prefix + name,
            target_name=name,
            module_path=project_path,
            use=[target_prefix + d for d in depends],
            private_use=[target_prefix + d for d in private_depends],
            features=features,
            extra_use=extra_features,
            defines=[
                'building_%s' % safe_name(name.split('.')[-1]),
                'BE_PROJECTID=%s' % name.replace('.', '_'),
                'BE_PROJECTNAME=%s' % name
            ] + extra_defines,
            export_defines=[] + extra_public_defines,
            includes=extra_includes + api + platform_api + include + platform_include + master_includes +
            [bld.bugenginenode],
            libs=[],
            lib_paths=lib_paths,
            export_includes=api + platform_api + extra_public_includes,
            frameworks=[],
            source=sources[:],
            pchstop=pchstop,
            preprocess=preprocess,
            export_all=export_all,
            source_nodes=[source_node] + [e for _, e in extras],
            root_namespace=root_namespace
        )
        result.append(task_gen)
        if target_prefix:
            internal_deps.append(target_prefix + name)
    multiarch = bld(target=name, features=['multiarch'], use=internal_deps) if internal_deps else None

    if multiarch or result:
        install_tg = multiarch if multiarch else result[0]
        for p, e in [('data', 'DEPLOY_DATADIR'), ('bin', 'DEPLOY_RUNBINDIR')]:
            node = source_node.make_node(p)
            if os.path.isdir(node.abspath()):
                install_tg.deploy_directory(bld.env, node, '', e)
            for tp in platforms:
                node = source_node.make_node(p + '.' + tp)
                if os.path.isdir(node.abspath()):
                    install_tg.deploy_directory(bld.env, node, '', e)
                for a in archs:
                    node = source_node.make_node(p + '.' + tp + '.' + a)
                    if os.path.isdir(node.abspath()):
                        install_tg.deploy_directory(bld.env, node, '', e)

    return (result, multiarch)


def build(build_context):
    pass