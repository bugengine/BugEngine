from waflib.Configure import conf


@conf
def engine(
    bld,
    name,
    depends=[],
    private_use=[],
    features=[],
    platforms=[],
    extra_includes=[],
    extra_defines=[],
    extra_public_includes=[],
    extra_public_defines=[],
    extra_tasks=[],
    path=None,
    use_master=True,
    warnings=True,
    root_namespace='BugEngine'
):
    if getattr(bld, 'launcher', None) != None:
        raise Errors.WafError('Only one engine can be defined')
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    bld.launcher = bld.module(
        name, path, depends + ['3rdparty.system.console'], private_use, platforms,
        extra_tasks + ['cxx', 'cxxprogram', 'launcher'], features, extra_includes, extra_defines, extra_public_includes,
        extra_public_defines, use_master, warnings, False, root_namespace
    )
    if 'windows' in bld.env.VALID_PLATFORMS:
        bld.module(
            name + 'w', path, depends, private_use, platforms, extra_tasks + ['cxx', 'cxxprogram', 'launcher'],
            features, extra_includes, extra_defines, extra_public_includes, extra_public_defines, use_master, warnings,
            False, root_namespace
        )


def build(build_context):
    pass