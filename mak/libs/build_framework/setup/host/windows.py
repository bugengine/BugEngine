import os


def setup(conf):
    path = conf.bldnode.make_node('host/win32')
    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([os.path.join(path.abspath(), 'bin'), environ['PATH']])
