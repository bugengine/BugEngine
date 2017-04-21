from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python33 = True
        conf.env.check_python33_defines = ['PYTHON_LIBRARY="python33"']
    else:
        raise Errors.WafError('python 3.3 not available')

