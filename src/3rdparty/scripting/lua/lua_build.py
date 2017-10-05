from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_LUA or Options.options.included_lua or bld.env.PROJECTS:
        bld.static_library('3rdparty.scripting.lua', path='.',
                           extra_defines=['LUA_LIB'],
                           use_master=False, warnings=False)
    else:
        bld.thirdparty('3rdparty.scripting.lua')
