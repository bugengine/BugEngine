
def options(opt):
    pass

def setup(conf):
    pass

def build(bld):
    bld.static_library('3rdparty.lua', extra_defines=['LUA_LIB'], warnings=False)