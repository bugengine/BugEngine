
def options(opt):
    pass

def setup(conf):
    pass

def build(bld):
    bld.static_library('3rdparty.squirrel', bld.platforms, extra_defines=['_BSD_SOURCE'], warnings=False)
