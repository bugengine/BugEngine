import os
from waflib import Errors

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        pass
    elif 'macosx' in conf.env.VALID_PLATFORMS:
        pass
    else:
        raise Errors.WafError('python not available')

