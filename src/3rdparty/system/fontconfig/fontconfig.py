from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('fontconfig', var='fontconfig')
        except Exception as e:
            pprint('YELLOW', '-fontconfig', sep=' ')
        else:
            pprint('GREEN', '+fontconfig', sep=' ')
            conf.env.SYSTEM_FONTCONFIG = True
