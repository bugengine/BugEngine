import os
from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL',
                             includepath=[os.path.join(conf.path.abspath(), 'api')]) or pprint('YELLOW', '-GL', sep=' ')
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['opengl32', 'gdi32'],
                       includes=['windows.h', 'GL/gl.h'],
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       functions=['glBegin']) or pprint('YELLOW', '-GL', sep=' ')
    else:
        try:
            conf.pkg_config('gl', var='OpenGL')
            conf.env.append_unique('check_OpenGL_includes', [os.path.join(conf.path.abspath(), 'api')])
        except Exception as e:
            conf.check_lib('GL', var='OpenGL',
                           includes=['GL/gl.h'],
                           includepath=[os.path.join(conf.path.abspath(), 'api')],
                           functions=['glBegin']) or pprint('YELLOW', '-GL', sep=' ')
        else:
            conf.env.SYSTEM_OPENGL = True
            pprint('GREEN', '+GL', sep=' ')
