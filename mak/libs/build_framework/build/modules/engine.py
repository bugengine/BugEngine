from waflib.Configure import conf
from waflib import Errors


@conf
def engine(
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
        if getattr(bld, 'launcher', None) != None:
            raise Errors.WafError('Only one engine launcher can be defined')
        p = bld.preprocess(name, path, root_namespace, 'bugengine')
        bld.launcher = bld.multiarch(
            name, [
                engine(
                    bld, name, depends, private_depends, path, features, extra_includes, extra_defines,
                    extra_public_includes, extra_public_defines, source_list, conditions, root_namespace, env
                ) for env in bld.multiarch_envs
            ]
        )
        if 'windows' in bld.env.VALID_PLATFORMS:
            bld.preprocess(name + '.console', p.source_nodes[0], root_namespace, 'bugengine')
            bld.multiarch(
                name + '.console', [
                    engine(
                        bld, name + '.console', depends, private_depends + ['console'], p.source_nodes[0], features,
                        extra_includes, extra_defines, extra_public_includes, extra_public_defines, source_list,
                        conditions, root_namespace, env
                    ) for env in bld.multiarch_envs
                ]
            )
        return bld.launcher
    else:
        features = features + ['c', 'cxx', 'cxxprogram', 'bugengine:c', 'bugengine:cxx', 'bugengine:launcher']
        return bld.module(**locals())


def build(build_context):
    pass
