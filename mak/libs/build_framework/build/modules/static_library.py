
from waflib.Configure import conf


@conf
def static_library(
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
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return None
    return bld.module(
        name, path, depends, private_use, platforms, extra_tasks + ['cxx', 'cxxstlib'], features, extra_includes,
        extra_defines, extra_public_includes, extra_public_defines, use_master, warnings, False, root_namespace
    )


def build(build_context):
    pass