import os


def build(bld):
    environ = getattr(bld, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([bld.bugenginenode.make_node("mak/host/win32/bin").abspath(), environ['PATH']])