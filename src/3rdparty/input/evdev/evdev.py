from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    try:
        conf.pkg_config('libevdev', vars='evdev')
    except Exception:
        pprint('YELLOW', '-evdev', sep=' ')
    else:
        pprint('GREEN', '+evdev', sep=' ')
