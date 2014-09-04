import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGLES', var='GLESFramework')
    else:
        conf.check_lib('GLESv2', var='GLES2', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])
        conf.check_lib('EGL', var='EGL', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])

