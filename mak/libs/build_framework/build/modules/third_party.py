from waflib.Configure import conf
import os


@conf
def thirdparty(bld, name, feature='', path='', var='', use=[], private_use=[]):
    platforms = bld.env.VALID_PLATFORMS
    platform_specific = platforms
    source_node = bld.path.parent.make_node(path and path.replace('.', '/') or '.')
    project_path = source_node.parent.path_from(bld.srcnode).replace('/', '.').replace('\\', '.')
    project_path = '%s.%s' % (project_path, name.split('.')[-1])
    if not var:
        var = bld.path.parent.name

    internal_deps = []
    supported = False
    for env in bld.multiarch_envs:
        target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
        target_name = target_prefix + name
        if env['check_%s' % var] or env.PROJECTS:
            if feature:
                bld.add_feature(feature, env)
            supported = True
            tg = bld(
                target=target_name,
                features=['cxx'],
                module_path=project_path,
                export_includes=env['check_%s_includes' % var],
                export_defines=env['check_%s_defines' % var] + ['BE_HAVE_%s' % var.upper()],
                export_libpath=env['check_%s_libpath' % var],
                export_lib=env['check_%s_libs' % var],
                export_framework=env['check_%s_frameworks' % var],
                export_cflags=env['check_%s_cflags' % var],
                export_cxxflags=env['check_%s_cxxflags' % var],
                export_linkflags=env['check_%s_ldflags' % var],
                source_nodes=[source_node],
                use=[target_prefix + u for u in use],
                private_use=[target_prefix + u for u in private_use]
            )
            if target_prefix:
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
                tg.deploy_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
            for data_path in data_paths:
                tg.deploy_directory(env, data_path, '', 'DEPLOY_DATADIR')
            if name not in bld.env.THIRDPARTIES_FIRST:
                bin_paths = [
                    i for i in [source_node.make_node('bin')] +
                    [source_node.make_node('bin.%s' % platform) for platform in platform_specific]
                    if os.path.isdir(i.abspath())
                ]
                for bin_path in bin_paths:
                    tg.deploy_directory(env, bin_path, '', 'DEPLOY_RUNBINDIR')
                bld.env.append_unique('THIRDPARTIES_FIRST', name)

    if supported:
        if internal_deps:
            tg = bld(target=name, features=['multiarch'], use=[d.name for d in internal_deps])

        if name not in bld.env.THIRDPARTIES_FIRST:
            bin_paths = [
                i for i in [source_node.make_node('bin')] +
                [source_node.make_node('bin.%s' % platform) for platform in platform_specific]
                if os.path.isdir(i.abspath())
            ]
            for bin_path in bin_paths:
                tg.deploy_directory(bld.env, bin_path, '', 'DEPLOY_RUNBINDIR')
            bld.env.append_unique('THIRDPARTIES_FIRST', name)
        return tg
    else:
        return None


def build(build_context):
    pass
