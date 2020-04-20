import os


def setup(conf):
    paths = [os.path.join(conf.path.parent.abspath(), 'bin.%s'%a) for a in conf.env.VALID_ARCHITECTURES]
    paths += [os.path.join(conf.path.parent.abspath(), 'lib.%s'%a) for a in conf.env.VALID_ARCHITECTURES]
    paths = [p for p in paths if os.path.isdir(p)]
    conf.check_lib('c++', var='libc++',
                   libpath=paths,
                   includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                   includes=['cassert'])
