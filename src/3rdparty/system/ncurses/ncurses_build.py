def build(bld):
    if not bld.env.SYSTEM_NCURSES or bld.env.PROJECTS:
        includes = [bld.path.make_node('src/ncurses')]
        bld.static_library('3rdparty.system.ncurses', path='.', extra_includes=includes,
                           warnings=False, use_master=False)
        bld.add_feature('ncurses')
    else:
        bld.thirdparty('3rdparty.system.ncurses', feature='ncurses')
