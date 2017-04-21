from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python31 = True
        conf.env.check_python31_defines = ['PYTHON_LIBRARY="python31"']
    else:
        raise Errors.WafError('python 3.1 not available')

