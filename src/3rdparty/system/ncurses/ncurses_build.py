from waflib import Options

def build(bld):
    if ('windows' in bld.env.VALID_PLATFORMS and (not bld.env.SYSTEM_NCURSES
                                                  or Options.options.included_curses)) or bld.env.PROJECTS:
        includes = [bld.path.make_node('src/ncurses')]
        bld.shared_library('3rdparty.system.ncurses', path='.',
                           depends=['3rdparty.system.win32'],
                           extra_includes=includes,
                           extra_defines=['PDC_DLL_BUILD'],
                           warnings=False, use_master=False)
        bld.add_feature('ncurses')
    else:
        bld.thirdparty('3rdparty.system.ncurses', feature='ncurses')
