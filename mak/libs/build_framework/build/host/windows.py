import os


def build(bld):
    environ = getattr(bld, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([bld.bldnode.parent.parent.make_node("host/win32/bin").abspath(), environ['PATH']])
    if bld.env.PATH:
        bld.env.PATH = [bld.bldnode.parent.parent.make_node("host/win32/bin").abspath()] + bld.env.PATH
