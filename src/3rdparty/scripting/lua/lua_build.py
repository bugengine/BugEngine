from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_LUA or Options.options.included_lua or bld.env.PROJECTS:
        dll_flags = 'windows' in bld.env.PLATFORM and ['LUA_BUILD_AS_DLL'] or []
        bld.static_library('3rdparty.scripting.lua', path='.',
                           extra_defines=['LUA_LIB'] + dll_flags,
                           extra_includes=[bld.path.make_node('src/core'),
                                           bld.path.make_node('src/lualib')],
                           use_master='folder', warnings=False)
    else:
        bld.thirdparty('3rdparty.scripting.lua')
