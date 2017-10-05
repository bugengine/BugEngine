from waflib.Logs import pprint


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-zlib',
                  action='store_true',
                  default=False,
                  dest='included_zlib',
                  help='Compile the zlib library from the included code')


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
