from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_BULLET or Options.options.included_bullet or bld.env.PROJECTS:
        bld.static_library(
            '3rdparty.physics.bullet',
            bld.platforms,
            path=bld.path.parent,
            extra_includes=[bld.path.make_node('src')],
            extra_public_includes=[bld.path.make_node('src')],
            extra_defines=['BT_NO_PROFILE'],
            warnings=False)
    else:
        bld.thirdparty('3rdparty.physics.bullet')
