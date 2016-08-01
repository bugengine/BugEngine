import os

def options(opt):
    pass

def setup(conf):
    conf.check_lib(['psapi', 'dbghelp', 'version'], var='PSAPI',
                   libpath=[os.path.join(conf.path.abspath(),
                   'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES],
                   includes=['dbghelp.h'],
                   functions=['SymInitialize(0, 0, 0)'])
