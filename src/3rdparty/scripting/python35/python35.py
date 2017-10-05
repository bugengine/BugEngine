from waflib import Errors
import os

def options(opt):
    pass

def setup(conf):
    pass

def setup_python(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        for a in conf.env.VALID_ARCHITECTURES:
            if os.path.isdir(os.path.join(conf.path.abspath(), 'bin.windows.%s' % a)):
                conf.env.check_python34 = True
                conf.env.check_python34_defines = ['PYTHON_LIBRARY="python34"']
                break
        else:
            raise Errors.WafError('%s not available for windows %s' % (conf.path.name,
                                                                       conf.env.VALID_ARCHITECTURES[0]))
    elif 'macosx' in conf.env.VALID_PLATFORMS:
        conf.env.check_python35 = True
        conf.env.check_python35_defines = ['PYTHON_LIBRARY="python3.5m"']
    else:
        raise Errors.WafError('%s not available' % conf.path.name)