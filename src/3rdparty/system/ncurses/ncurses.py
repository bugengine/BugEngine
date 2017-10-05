from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        if not conf.check_lib(['ncurses', 'panel', 'form', 'menu']):
            pprint('YELLOW','ncurses',sep=' ')
            pprint('YELLOW','panel',sep=' ')
            pprint('YELLOW','form',sep=' ')
            pprint('YELLOW','menu',sep=' ')
    else:
        pprint('BLUE', '=curses', sep=' ')
