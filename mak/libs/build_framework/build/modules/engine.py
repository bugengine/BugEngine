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
            raise Errors.WafError('Only one engine can be defined')
        bld.preprocess(name, path, root_namespace, 'bugengine')
        bld.launcher = bld.multiarch(
            name, [
                engine(
                    bld, name, depends + ['3rdparty.system.console'], private_depends, path, features, extra_includes,
                    extra_defines, extra_public_includes, extra_public_defines, source_list, conditions, root_namespace,
                    env
                ) for env in bld.multiarch_envs
            ]
        )
        if 'windows' in bld.env.VALID_PLATFORMS:
            bld.preprocess(name + 'w', path, root_namespace, 'bugengine')
            bld.multiarch(
                name, [
                    engine(
                        bld, name + 'w', depends, private_depends, path, features, extra_includes, extra_defines,
                        extra_public_includes, extra_public_defines, source_list, conditions, root_namespace, env
                    ) for env in bld.multiarch_envs
                ]
            )
        return bld.launcher
    else:
        features = features + ['c', 'cxx', 'cxxprogram', 'bugengine:c', 'bugengine:cxx', 'bugengine:launcher']
        return bld.module(**locals())


def build(build_context):
    pass