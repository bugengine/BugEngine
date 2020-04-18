import os


def configure(conf):
    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([conf.bugenginenode.make_node("mak/host/win32/bin").abspath(), environ['PATH']])
