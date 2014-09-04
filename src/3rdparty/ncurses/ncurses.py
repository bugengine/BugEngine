def options(opt):
    pass

def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['ncurses', 'panel', 'form', 'menu'], var='NCurses')
