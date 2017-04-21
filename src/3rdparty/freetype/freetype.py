from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('freetype2', var='3rdparty.freetype')
        except Exception as e:
            pprint('BLUE', '=freetype', sep=' ')
        else:
            conf.env.SYSTEM_FREETYPE = True
            pprint('GREEN', '+freetype', sep=' ')
    else:
        pprint('BLUE', '=freetype', sep=' ')
