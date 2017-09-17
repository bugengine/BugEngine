def options(opt):
    pass

def setup(conf):
    try:
        conf.pkg_config('libevdev', vars='3rdparty.evdev')
    except Exception:
        pass
    else:
        pprint('GREEN', '+evdev', sep=' ')
