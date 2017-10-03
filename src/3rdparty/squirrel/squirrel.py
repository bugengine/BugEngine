from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    try:
        conf.pkg_config('squirrel3', var='3rdparty.squirrel')
    except Exception as e:
        pprint('BLUE', '=squirrel', sep=' ')
    else:
        conf.env.SYSTEM_SQUIRREL = True
        pprint('GREEN', '+squirrel', sep=' ')
