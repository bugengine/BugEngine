from waflib.Logs import pprint


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-bullet',
                  action='store_true',
                  default=False,
                  dest='included_bullet',
                  help='Compile the bullet library from the included code')


def setup(conf):
    if conf.env.CXX_NAME != 'sun':
        try:
            conf.pkg_config('bullet')
        except Exception as e:
            pprint('BLUE', '=bullet', sep=' ')
        else:
            conf.env.SYSTEM_BULLET = True
            pprint('GREEN', '+bullet', sep=' ')
    else:
        pprint('BLUE', '=bullet', sep=' ')
