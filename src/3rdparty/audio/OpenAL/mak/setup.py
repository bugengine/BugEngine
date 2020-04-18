import os
from waflib.Logs import pprint


def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenAL')
    else:
        try:
            conf.pkg_config('openal', var='OpenAL')
        except Exception as e:
            conf.check_lib('openal', var='OpenAL',
                           libpath=[os.path.join(conf.path.abspath(),
                                                 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                    for a in conf.env.VALID_ARCHITECTURES],
                           includepath=[os.path.join(conf.path.abspath(), 'api')],
                           includes=['AL/alc.h', 'AL/al.h'],
                           functions=['alcOpenDevice']) or pprint('YELLOW', '-openal', sep=' ')
        else:
            conf.env.SYSTEM_OPENAL = True
            pprint('GREEN', '+openal', sep=' ')
