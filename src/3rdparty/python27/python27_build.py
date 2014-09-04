import os
from waflib import Errors

def build(bld):
    if 'macosx' in bld.env.VALID_PLATFORMS:
        for env in bld.multiarch_envs:
            bld.thirdparty('3rdparty.python27', env,
                           defines=['PYTHON_LIBRARY="python2.7"'])
    elif 'windows' in bld.env.VALID_PLATFORMS:
        bld.thirdparty('3rdparty.python27', bld.env,
                       defines=['PYTHON_LIBRARY="python27"'])
    else:
        raise Errors.WafError('python not available')

