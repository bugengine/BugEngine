import os

def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        include = 'OpenCL/cl.h'
        libpath=[os.path.join(conf.path.abspath(), 'lib.%s'%(conf.env.VALID_PLATFORMS[0]))]
    else:
        include = 'CL/cl.h'
        libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a))
                 for a in conf.env.VALID_ARCHITECTURES]
    conf.check_lib('OpenCL',
                   includes=[include],
                   libpath=libpath,
                   includepath=[os.path.join(conf.path.abspath(), 'api')],
                   functions=['clGetDeviceInfo'])

def build(bld):
    for env in bld.multiarch_envs:
        if env.OpenCL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenCL',
                env,
                libs=env.OpenCL,
                includes=[bld.path.find_node('api')])
            env.append_unique('FEATURES', ['OpenCL'])

