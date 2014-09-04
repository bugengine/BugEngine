import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenAL', var='ALFramework')
    else:
        conf.check_lib('openal', var='OpenAL', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])
