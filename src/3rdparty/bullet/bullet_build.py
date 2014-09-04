def build(bld):
    bld.static_library(
        '3rdparty.bullet',
        bld.platforms,
        extra_includes=[bld.path.make_node('src')],
        extra_public_includes=[bld.path.make_node('src')],
        extra_defines=['BT_NO_PROFILE'],
        warnings=False)
