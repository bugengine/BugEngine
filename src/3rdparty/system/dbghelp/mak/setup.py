import os
from waflib.Logs import pprint


def setup(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        if not conf.check_lib(['psapi', 'dbghelp', 'version'],
                              includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                              libpath=[os.path.join(conf.path.parent.abspath(),
                              'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES],
                              includes=['windows.h', 'dbghelp.h'],
                              functions=['SymInitialize']):
            pprint('YELLOW', '-psapi', sep=' ')
            pprint('YELLOW', '-dbghelp', sep=' ')
            pprint('YELLOW', '-version', sep=' ')
