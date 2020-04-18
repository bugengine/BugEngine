from waflib.Logs import pprint
import os


def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.SYSTEM_NCURSES = conf.check_lib(['ncurses', 'panel', 'form', 'menu'])
        if not conf.env.SYSTEM_NCURSES:
            pprint('YELLOW','ncurses',sep=' ')
            pprint('YELLOW','panel',sep=' ')
            pprint('YELLOW','form',sep=' ')
            pprint('YELLOW','menu',sep=' ')
    else:
        conf.env.SYSTEM_NCURSES = conf.check_lib('3rdparty.system.ncurses', var='ncurses',
                       libpath=[os.path.join(conf.path.parent.abspath(),
                                            'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.parent.abspath(), 'api.windows')],
                       includes=['curses.h'],
                       defines=['PDC_DLL_BUILD'],
                       functions=['initscr'])
        if not conf.env.SYSTEM_NCURSES:
            pprint('BLUE', '=curses', sep=' ')
