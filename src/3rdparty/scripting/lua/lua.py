from waflib.Logs import pprint
import os


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
        conf.env.SYSTEM_LUA = conf.check_lib('lua', var='lua',
                       libpath=[os.path.join(conf.path.abspath(),
                                            'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       includes_externc=['lua.h', 'lauxlib.h'],
                       defines=['LUA_LIB'],
                       functions=['luaL_newmetatable'])
        if not conf.env.SYSTEM_LUA:
            pprint('BLUE', '=lua', sep=' ')

