from waflib.Logs import pprint
import os


def setup(conf):
    if conf.env.CXX_NAME != 'sun' and 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('bullet')
        except Exception as e:
            pprint('BLUE', '=bullet', sep=' ')
        else:
            conf.env.SYSTEM_BULLET = True
            pprint('GREEN', '+bullet', sep=' ')
    else:
        conf.env.SYSTEM_BULLET = conf.check_lib('bullet', var='bullet',
                       libpath=[os.path.join(conf.path.abspath(),
                                            'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                                for a in conf.env.VALID_ARCHITECTURES],
                       includepath=[os.path.join(conf.path.abspath(), 'src')],
                       includes=['btBulletDynamicsCommon.h', 'LinearMath/btAlignedAllocator.h'],
                       functions=['btAlignedAllocSetCustom'])
        if not conf.env.SYSTEM_BULLET:
            pprint('BLUE', '=bullet', sep=' ')
