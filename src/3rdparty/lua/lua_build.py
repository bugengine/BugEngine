def build(bld):
    bld.static_library('3rdparty.lua', path='.', extra_defines=['LUA_LIB'], warnings=False)
