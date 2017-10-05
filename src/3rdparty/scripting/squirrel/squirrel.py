from waflib.Logs import pprint


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-squirrel',
                  action='store_true',
                  default=False,
                  dest='included_squirrel',
                  help='Compile the squirrel library from the included code')


def setup(conf):
    try:
        conf.pkg_config('squirrel3', var='squirrel')
    except Exception as e:
        include = 'squirrel.h'
        if conf.check_lib('squirrel',
                          includes=[include],
                          functions=['sq_open']):
            conf.env.SYSTEM_SQUIRREL = True
        else:
            pprint('BLUE', '=squirrel', sep=' ')
    else:
        conf.env.SYSTEM_SQUIRREL = True
        pprint('GREEN', '+squirrel', sep=' ')
