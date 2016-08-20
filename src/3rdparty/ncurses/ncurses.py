from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['ncurses', 'panel', 'form', 'menu'], var='curses')
    else:
        pprint('BLUE', '=curses', sep=' ')
