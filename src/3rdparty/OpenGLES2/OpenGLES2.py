import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGLES', var='GLESFramework')
    else:
        conf.check_lib('GLESv2', var='GLES2', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])
        conf.check_lib('EGL', var='EGL', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])

def build(bld):
    for env in bld.multiarch_envs:
        if env.GLESFramework or env.GLES2 or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenGLES2', env, libs=env.GLES2+env.EGL, frameworks=env.GLESFramework)
            env.append_unique('FEATURES', ['OpenGLES2'])
