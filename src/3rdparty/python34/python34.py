from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python34 = True
        conf.env.check_python34_defines = ['PYTHON_LIBRARY="python34"']
    else:
        raise Errors.WafError('python 3.4 not available')

