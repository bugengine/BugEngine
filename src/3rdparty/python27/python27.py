import os
from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.env.check_python27 = True
        conf.env.check_python27_defines = ['PYTHON_LIBRARY="python27"']
    elif 'macosx' in conf.env.VALID_PLATFORMS:
        conf.env.check_python27 = True
        conf.env.check_python27_defines = ['PYTHON_LIBRARY="python2.7"']
    else:
        raise Errors.WafError('python not available')

