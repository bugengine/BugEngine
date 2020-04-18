from waflib.Logs import pprint
import os


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
        conf.env.SYSTEM_FREETYPE = conf.check_lib('freetype', var='freetype',
                       libpath=[os.path.join(conf.path.parent.abspath(),
                                            'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                       includes=['ft2build.h', 'freetype/freetype.h', 'freetype/t1tables.h'],
                       functions=['FT_Get_PS_Font_Info'])
        if not conf.env.SYSTEM_FREETYPE:
            pprint('BLUE', '=freetype', sep=' ')
