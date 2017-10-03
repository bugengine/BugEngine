from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    try:
        conf.pkg_config('bullet', var='3rdparty.bullet')
    except Exception as e:
        pprint('BLUE', '=bullet', sep=' ')
    else:
        conf.env.SYSTEM_BULLET = True
        pprint('GREEN', '+bullet', sep=' ')
