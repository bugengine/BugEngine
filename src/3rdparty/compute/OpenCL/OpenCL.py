import os
from waflib.Logs import pprint


def options(opt):
    pass


def setup_shipped(conf):
    include = 'CL/cl.h'
    libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
             for a in conf.env.VALID_ARCHITECTURES]
    conf.check_lib('OpenCL',
                   includes=[include],
                   libpath=libpath,
                   includepath=[os.path.join(conf.path.abspath(), 'api')],
                   functions=['clGetDeviceInfo']) or pprint('YELLOW', '-OpenCL', sep=' ')


def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        include = 'OpenCL/cl.h'
        libpath=[os.path.join(conf.path.abspath(), 'lib.%s'%(conf.env.VALID_PLATFORMS[0]))]
        conf.check_lib('OpenCL',
                       includes=[include],
                       libpath=libpath,
                       includepath=[os.path.join(conf.path.abspath(), 'api')],
                       functions=['clGetDeviceInfo']) or pprint('YELLOW', '-OpenCL', sep=' ')
    elif conf.env.CC_NAME == 'icc':
        setup_shipped(conf)
    else:
        try:
            conf.pkg_config('OpenCL')
        except Exception as e:
            setup_shipped(conf)
        else:
            conf.env.SYSTEM_OPENCL = True
            pprint('GREEN', '+OpenCL', sep=' ')

