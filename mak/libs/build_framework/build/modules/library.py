from waflib.Configure import conf


@conf
def library(
    bld,
    name,
    depends=[],
    private_depends=[],
    path=None,
    features=[],
    extra_includes=[],
    extra_defines=[],
    extra_public_includes=[],
    extra_public_defines=[],
    source_list=None,
    conditions=[],
    root_namespace='BugEngine',
    env=None
):
    if env is None:
        bld.preprocess(name, path, root_namespace, 'bugengine')
        bld.multiarch(
            name, [
                library(
                    bld, name, depends, private_depends, path, features, extra_includes, extra_defines,
                    extra_public_includes, extra_public_defines, source_list, conditions, root_namespace, env
                ) for env in bld.multiarch_envs
            ]
        )
    else:
        features = features + (
            bld.env.DYNAMIC and ['c', 'cxx', 'cxxshlib', 'bugengine:c', 'bugengine:cxx', 'bugengine:shared_lib']
            or ['c', 'cxx', 'cxxobjects', 'bugengine:c', 'bugengine:cxx']
        )
        return bld.module(**locals())


def build(build_context):
    pass