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

