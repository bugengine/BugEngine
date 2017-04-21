from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python26 = True
        conf.env.check_python26_defines = ['PYTHON_LIBRARY="python26"']
    else:
        raise Errors.WafError('python 2.6 not available')

