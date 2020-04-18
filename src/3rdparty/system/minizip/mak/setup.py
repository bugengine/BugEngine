from waflib.Logs import pprint


def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('minizip')
        except Exception as e:
            pprint('BLUE', '=minizip', sep=' ')
        else:
            conf.env.SYSTEM_MINIZIP = True
            pprint('GREEN', '+minizip', sep=' ')
    else:
        pprint('BLUE', '=minizip', sep=' ')
