from waflib.Logs import pprint


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-lua',
                  action='store_true',
                  default=False,
                  dest='included_lua',
                  help='Compile the LUA library from the included code')


def setup(conf):
    for v in '-5.3', '5.3', '-5.2', '5.2', '-5.1', '5.1':
        try:
            conf.pkg_config('lua%s' % v, var='lua')
        except Exception as e:
            pass
        else:
            conf.env.SYSTEM_LUA = True
            pprint('GREEN', '+lua%s'%v, sep=' ')
            break
    else:
        pprint('BLUE', '=lua', sep=' ')

