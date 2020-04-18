import os
from waflib.Logs import pprint


def setup_shipped(conf):
    include = 'CL/cl.h'
    libpath=[os.path.join(conf.path.parent.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
             for a in conf.env.VALID_ARCHITECTURES]
    if conf.check_lib('OpenCL',
                      includes=[include],
                      libpath=libpath,
                      includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                      functions=['clGetDeviceInfo']):
        conf.env.append_value('KERNEL_TOOLCHAINS', [('opencl', conf.env.TOOLCHAIN)])
    else:
        pprint('YELLOW', '-OpenCL', sep=' ')


def setup(conf):
    if conf.env.CLC_CXX:
        if 'darwin' in conf.env.VALID_PLATFORMS:
            include = 'OpenCL/cl.h'
            libpath=[os.path.join(conf.path.parent.abspath(), 'lib.%s'%(conf.env.VALID_PLATFORMS[0]))]
            if conf.check_lib('OpenCL',
                              includes=[include],
                              libpath=libpath,
                              includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                              functions=['clGetDeviceInfo']):
                conf.env.append_value('KERNEL_TOOLCHAINS', [('opencl', conf.env.TOOLCHAIN)])
            else:
                pprint('YELLOW', '-OpenCL', sep=' ')
        elif conf.env.CC_NAME == 'icc':
            setup_shipped(conf)
        else:
            try:
                conf.pkg_config('OpenCL')
            except Exception as e:
                setup_shipped(conf)
            else:
                conf.env.SYSTEM_OPENCL = True
                conf.env.append_value('KERNEL_TOOLCHAINS', [('opencl', conf.env.TOOLCHAIN)])
                pprint('GREEN', '+OpenCL', sep=' ')
    else:
        pprint('YELLOW', '-OpenCL', sep=' ')
