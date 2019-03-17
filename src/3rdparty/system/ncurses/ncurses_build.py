def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS and (not bld.env.SYSTEM_NCURSES or bld.env.PROJECTS):
        includes = [bld.path.make_node('src/ncurses')]
        bld.shared_library('3rdparty.system.ncurses', path='.',
                           extra_includes=includes,
                           extra_defines=['NCURSES_DLL'],
                           warnings=False, use_master=False)
        bld.add_feature('ncurses')
    else:
        bld.thirdparty('3rdparty.system.ncurses', feature='ncurses')
