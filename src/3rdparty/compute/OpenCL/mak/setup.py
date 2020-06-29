from waflib.Logs import pprint
from waflib.Errors import WafError
from waflib import Options
import os

CL_ICD_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-opencl/OpenCL-icd-2.2-%(platform)s.tgz'


def setup(conf):
    if conf.env.CLC_CXX:
        if conf.env.PROJECTS:
            pprint('BLUE', '=OpenCL', sep=' ')
        else:
            if 'posix' in conf.env.VALID_PLATFORMS:
                try:
                    conf.pkg_config('OpenCL', var='OpenCL')
                except Exception as e:
                    pass
                else:
                    conf.env.OPENCL_BINARY = True
                    pprint('GREEN', '+OpenCL', sep=' ')
                    return
            try:
                cl_node = conf.pkg_unpack('cl_bin_%(platform)s', CL_ICD_BINARIES)
                if not conf.check_package('OpenCL', cl_node, var='OpenCL'):
                    raise WafError('no OpenCL')
                conf.env.OPENCL_BINARY = cl_node.path_from(conf.package_node)
                #pprint('GREEN', '+OpenCL', sep=' ')
            except WafError:
                pprint('YELLOW', '-OpenCL', sep=' ')
    else:
        pprint('YELLOW', '-OpenCL', sep=' ')