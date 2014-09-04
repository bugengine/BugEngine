from waflib import Errors

def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS:
        bld.thirdparty('3rdparty.python26', bld.env,
                       defines=['PYTHON_LIBRARY="python26"'])
    else:
        raise Errors.WafError('python not available')

