from waflib import Errors
import os


def setup(conf):
    pass


def setup_python(conf):
    python_name = conf.path.parent.name
    if 'windows' in conf.env.VALID_PLATFORMS:
        for a in conf.env.VALID_ARCHITECTURES:
            if os.path.isdir(os.path.join(conf.path.abspath(), 'bin.windows.%s' % a)):
                conf.env['check_%s' % python_name] = True
                conf.env['check_%s_defines' % python_name] = ['PYTHON_LIBRARY=%s' % python_name]
                break
        else:
            raise Errors.WafError('%s not available for windows %s' % (python_name,
                                                                       conf.env.VALID_ARCHITECTURES[0]))
    else:
        raise Errors.WafError('%s not available' % python_name)
