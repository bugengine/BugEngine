import os
from waflib.Logs import pprint


def options(opt):
    pass


def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGLES')
    else:
        try:
            conf.pkg_config('glesv2', var='OpenGLES2')
            conf.pkg_config('egl', var='OpenGLES2')
        except Exception as e:
            if not conf.check_lib(['GLESv2', 'EGL'], var='OpenGLES2',
                                  libpath=[os.path.join(conf.path.abspath(),
                                  'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES],
                                  includepath=[os.path.join(conf.path.abspath(), 'api')],
                                  includes=['GLES2/gl2.h', 'EGL/egl.h'],
                                  functions=['glGenFramebuffers', 'eglCreateContext']):
                pprint('YELLOW', '-GLESv2', sep=' ')
                pprint('YELLOW', '-EGL', sep=' ')
        else:
            conf.env.SYSTEM_OPENGLES2 = True
            pprint('GREEN', '+GLESv2', sep=' ')
            pprint('GREEN', '+EGL', sep=' ')