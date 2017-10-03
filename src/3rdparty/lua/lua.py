from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    for v in '-5.3', '5.3', '-5.2', '5.2', '-5.1', '5.1':
        try:
            conf.pkg_config('lua%s' % v, var='3rdparty.lua')
        except Exception as e:
            pass
        else:
            conf.env.SYSTEM_LUA = True
            pprint('GREEN', '+lua%s'%v, sep=' ')
            break
    else:
        pprint('BLUE', '=lua', sep=' ')

