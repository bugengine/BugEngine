import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenAL')
    else:
        conf.check_lib('openal',
                       libpath=[os.path.join(conf.path.abspath(),
                                             'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       includes=['AL/alc.h', 'AL/al.h'],
                       functions=['alcOpenDevice(0)'])
