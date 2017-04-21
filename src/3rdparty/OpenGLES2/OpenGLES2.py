import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGLES')
    else:
        conf.check_lib('GLESv2',
                       libpath=[os.path.join(conf.path.abspath(),
                       'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       includes=['GLES2/gl2.h'],
                       functions=['glGenFramebuffers(0, 0)'])
        conf.check_lib('EGL',
                       libpath=[os.path.join(conf.path.abspath(),
                       'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       includes=['EGL/egl.h'],
                       functions=['eglCreateContext(0, 0, 0, 0)'])
