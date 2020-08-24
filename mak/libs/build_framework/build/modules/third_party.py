from waflib.Configure import conf
import os


def expand_libpath(bld, libpath):
    result = []
    for l in libpath:
        if os.path.isdir(os.path.join(l, bld.__class__.optim)):
            result.append(os.path.join(l, bld.__class__.optim))
        else:
            result.append(l)
    return result


def expand_nodepath(bld, nodepath):
    result = []
    for n in nodepath:
        if n.make_node(bld.__class__.optim).isdir():
            result.append(n.make_node(bld.__class__.optim))
        else:
            result.append(n)
    return result


@conf
def thirdparty(bld, name, feature='', path='', source_node=None, var='', use=[], private_use=[], env=None):
    if not var:
        var = bld.path.parent.name
    platforms = bld.env.VALID_PLATFORMS
    platform_specific = platforms
    if source_node is None:
        source_node = bld.path.parent.make_node(path and path.replace('.', '/') or '.')
    if env is None:
        internal_deps = []
        supported = False
        for env in bld.multiarch_envs:
            target_name = env.ENV_PREFIX % name
            if env['check_%s' % var] or env.PROJECTS:
                if feature:
                    bld.add_feature(feature, env)
                supported = True
                var_id = var.upper().replace('+', 'P').replace('-', '_')
                tg = bld(
                    target=target_name,
                    features=['c', 'cxx'],
                    export_includes=env['check_%s_includes' % var],
                    export_defines=env['check_%s_defines' % var] + ['BE_HAVE_%s' % var_id],
                    export_libpath=expand_libpath(bld, env['check_%s_libpath' % var]),
                    export_lib=env['check_%s_libs' % var],
                    export_framework=env['check_%s_frameworks' % var],
                    export_cflags=env['check_%s_cflags' % var],
                    export_cxxflags=env['check_%s_cxxflags' % var],
                    export_linkflags=env['check_%s_ldflags' % var],
                    source_nodes=[source_node],
                    use=[env.ENV_PREFIX % u for u in use],
                    private_use=[env.ENV_PREFIX % u for u in private_use]
                )
                if env.SUBARCH:
                    internal_deps.append(tg)
                archs = env.VALID_ARCHITECTURES
                arch_specific = archs + ['%s.%s' % (p, a) for p in platforms for a in archs]
                bin_paths = [
                    i for i in [source_node.make_node('bin.%s' % arch) for arch in arch_specific]
                    if os.path.isdir(i.abspath())
                ]
                data_paths = [
                    i for i in [source_node.make_node('data.%s' % arch) for arch in arch_specific]
                    if os.path.isdir(i.abspath())
                ]
                for bin_path in bin_paths:
                    tg.install_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
                for data_path in data_paths:
                    tg.install_directory(env, data_path, '', 'DEPLOY_DATADIR')
                if name not in bld.env.THIRDPARTIES_FIRST:
                    bin_paths = [
                        i for i in [source_node.make_node('bin')] +
                        [source_node.make_node('bin.%s' % platform) for platform in platform_specific]
                        if os.path.isdir(i.abspath())
                    ]
                    for bin_path in bin_paths:
                        tg.install_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
                    bld.env.append_unique('THIRDPARTIES_FIRST', name)

        if supported:
            if internal_deps:
                tg = bld(target=name, features=['bugengine:multiarch'], use=[d.name for d in internal_deps])

            if name not in bld.env.THIRDPARTIES_FIRST:
                bin_paths = [
                    i for i in [source_node.make_node('bin')] +
                    [source_node.make_node('bin.%s' % platform) for platform in platform_specific]
                    if os.path.isdir(i.abspath())
                ]
                for bin_path in bin_paths:
                    tg.install_directory(bld.env, bin_path, '', 'DEPLOY_RUNBINDIR')
                bld.env.append_unique('THIRDPARTIES_FIRST', name)
            return tg
        else:
            return None
    else:
        target_name = env.ENV_PREFIX % name
        if env['check_%s' % var] or env.PROJECTS:
            if feature:
                bld.add_feature(feature, env)
            var_id = var.upper().replace('+', 'P').replace('-', '_')
            tg = bld(
                target=target_name,
                features=['c', 'cxx'],
                export_includes=env['check_%s_includes' % var],
                export_defines=env['check_%s_defines' % var] + ['BE_HAVE_%s' % var_id],
                export_libpath=expand_libpath(bld, env['check_%s_libpath' % var]),
                export_lib=env['check_%s_libs' % var],
                export_framework=env['check_%s_frameworks' % var],
                export_cflags=env['check_%s_cflags' % var],
                export_cxxflags=env['check_%s_cxxflags' % var],
                export_linkflags=env['check_%s_ldflags' % var],
                source_nodes=[source_node],
                use=[env.ENV_PREFIX % u for u in use],
                private_use=[env.ENV_PREFIX % u for u in private_use]
            )
            archs = env.VALID_ARCHITECTURES
            arch_specific = archs + ['%s.%s' % (p, a) for p in platforms for a in archs]
            bin_paths = expand_nodepath(
                bld, [
                    i for i in [source_node.make_node('bin.%s' % arch)
                                for arch in arch_specific] if os.path.isdir(i.abspath())
                ]
            )
            data_paths = [
                i for i in [source_node.make_node('data.%s' % arch) for arch in arch_specific]
                if os.path.isdir(i.abspath())
            ]
            for bin_path in bin_paths:
                tg.install_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
            for data_path in data_paths:
                tg.install_directory(env, data_path, '', 'DEPLOY_DATADIR')
            if name not in bld.env.THIRDPARTIES_FIRST:
                bin_paths = expand_nodepath(
                    bld, [
                        i for i in [source_node.make_node('bin')] +
                        [source_node.make_node('bin.%s' % platform)
                         for platform in platform_specific] if os.path.isdir(i.abspath())
                    ]
                )
                for bin_path in bin_paths:
                    tg.install_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
                bld.env.append_unique('THIRDPARTIES_FIRST', name)
            return tg
        else:
            return None


def build(build_context):
    pass
