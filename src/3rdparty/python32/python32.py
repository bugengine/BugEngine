from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        pass
    else:
        raise Errors.WafError('python 3.2 not available')

def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS:
        bld.thirdparty('3rdparty.python32', bld.env,
                       defines=['PYTHON_LIBRARY="python32"'])
    else:
        raise Errors.WafError('python not available')
