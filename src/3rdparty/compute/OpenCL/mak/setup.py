from waflib.Errors import WafError
from waflib import Options
import os

CL_ICD_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-opencl/OpenCL-icd-2.2-%(platform)s.tgz'


def setup(conf):
    if not conf.env.PROJECTS:
        conf.start_msg_setup()
        if conf.env.CLC_CXX:
            if 'posix' in conf.env.VALID_PLATFORMS:
                try:
                    conf.pkg_config('OpenCL', var='OpenCL')
                except WafError as e:
                    pass
                else:
                    conf.env.OPENCL_BINARY = True
                    conf.end_msg('from pkg-config', color='GREEN')
                    return
            if conf.check_lib('OpenCL', var='OpenCL'):
                conf.env.OPENCL_BINARY = True
                conf.end_msg('from system', color='GREEN')
                return
            try:
                cl_node = conf.pkg_unpack('cl_bin_%(platform)s', CL_ICD_BINARIES)
                if not conf.check_package('OpenCL', cl_node, var='OpenCL'):
                    raise WafError('no OpenCL')
                conf.env.OPENCL_BINARY = cl_node.path_from(conf.package_node)
                conf.end_msg('from prebuilt', color='GREEN')
            except WafError:
                conf.end_msg('not found', color='YELLOW')
        else:
            conf.end_msg('not found', color='YELLOW')
