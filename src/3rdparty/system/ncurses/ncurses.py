from waflib.Logs import pprint
import os


def options(opt):
    pass


def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.SYSTEM_NCURSES = conf.check_lib(['ncurses', 'panel', 'form', 'menu'])
        if not conf.env.SYSTEM_NCURSES:
            pprint('YELLOW','ncurses',sep=' ')
            pprint('YELLOW','panel',sep=' ')
            pprint('YELLOW','form',sep=' ')
            pprint('YELLOW','menu',sep=' ')
    else:
        conf.env.SYSTEM_NCURSES = conf.check_lib('ncurses', var='ncurses',
                       libpath=[os.path.join(conf.path.abspath(),
                                            'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'api.windows')],
                       includes=['curses.h'],
                       functions=['initscr'])
        if not conf.env.SYSTEM_NCURSES:
            pprint('BLUE', '=curses', sep=' ')
