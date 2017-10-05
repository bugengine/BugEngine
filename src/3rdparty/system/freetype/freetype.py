from waflib.Logs import pprint


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-freetype',
                  action='store_true',
                  default=False,
                  dest='included_freetype',
                  help='Compile the Freetype library from the included code')



def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('freetype2', var='freetype')
        except Exception as e:
            pprint('BLUE', '=freetype', sep=' ')
        else:
            conf.env.SYSTEM_FREETYPE = True
            pprint('GREEN', '+freetype', sep=' ')
    else:
        pprint('BLUE', '=freetype', sep=' ')
