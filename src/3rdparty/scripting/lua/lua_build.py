from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_LUA or Options.options.included_lua or bld.env.PROJECTS:
        bld.static_library('3rdparty.scripting.lua', path='.',
                           extra_defines=['LUA_LIB', 'LUA_BUILD_AS_DLL'],
                           extra_includes=[bld.path.make_node('src/core'),
                                           bld.path.make_node('src/lualib')],
                           use_master='folder', warnings=False)
    else:
        bld.thirdparty('3rdparty.scripting.lua')
