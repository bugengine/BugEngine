from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python32 = True
        conf.env.check_python32_defines = ['PYTHON_LIBRARY="python32"']
    else:
        raise Errors.WafError('python 3.2 not available')

