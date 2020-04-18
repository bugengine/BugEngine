from waflib.Logs import pprint


def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('zlib')
        except Exception as e:
            pprint('BLUE', '=zlib', sep=' ')
        else:
            conf.env.SYSTEM_ZLIB = True
            pprint('GREEN', '+zlib', sep=' ')
    else:
        pprint('BLUE', '=zlib', sep=' ')
