import os
from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL',
                             includepath=[os.path.join(conf.path.abspath(), 'api')])
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['opengl32', 'gdi32'],
                       includes=['GL/gl.h'],
                       functions=['glBegin'])
    else:
        try:
            conf.pkg_config('gl', var='OpenGL')
        except Exception as e:
            conf.check_lib('GL', var='OpenGL',
                           includes=['GL/gl.h'],
                           functions=['glBegin']) or pprint('YELLOW', '-GL', sep=' ')
        else:
            conf.env.SYSTEM_OPENGL = True
            pprint('GREEN', '+GL', sep=' ')
