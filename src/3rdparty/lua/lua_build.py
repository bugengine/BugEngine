def build(bld):
    if not bld.env.SYSTEM_LUA or bld.env.PROJECTS:
        bld.static_library('3rdparty.lua', path='.', extra_defines=['LUA_LIB'], warnings=False)
