def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS or bld.env.PROJECTS:
        includes = [bld.path.make_node('src/ncurses')]
        bld.static_library('3rdparty.ncurses', path='.', extra_includes=includes,
                           warnings=False, use_master=False)
        bld.add_feature('ncurses')
    else:
        bld.thirdparty('3rdparty.ncurses', feature='ncurses')
