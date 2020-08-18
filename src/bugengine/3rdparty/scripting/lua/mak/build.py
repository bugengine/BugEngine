from waflib.TaskGen import feature, after_method
import os

LUA_HEADERS = ['src/lauxlib.h', 'src/lua.h', 'src/luaconf.h', 'src/lualib.h']
LUA_SOURCES = [
    'src/lapi.c',
    'src/lcode.c',
    'src/lctype.c',
    'src/ldebug.c',
    'src/ldo.c',
    'src/ldump.c',
    'src/lfunc.c',
    'src/lgc.c',
    'src/llex.c',
    'src/lmem.c',
    'src/lobject.c',
    'src/lopcodes.c',
    'src/lparser.c',
    'src/lstate.c',
    'src/lstring.c',
    'src/ltable.c',
    'src/ltm.c',
    'src/lundump.c',
    'src/lvm.c',
    'src/lzio.c',
    'src/lauxlib.c',
    'src/lbaselib.c',
    'src/lbitlib.c',
    'src/lcorolib.c',
    'src/ldblib.c',
    'src/liolib.c',
    'src/lmathlib.c',
                       #'src/loslib.c',
    'src/lstrlib.c',
    'src/ltablib.c',
    'src/lutf8lib.c',
    'src/loadlib.c',
                       #'src/linit.c',
]


@feature('bugengine:deploy:lua')
@after_method('install_step')
@after_method('apply_link')
def deploy_lua_package(task_gen):
    if task_gen.env.PROJECTS:
        return
    path = task_gen.source_nodes[0]
    lua_dest = 'lua-5.3.5-%s-multiarch-%s' % (task_gen.env.VALID_PLATFORMS[0], task_gen.env.COMPILER_ABI)
    src = path.make_node('src')

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld/packages', lua_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld/packages', lua_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        for h in path.ant_glob(LUA_HEADERS):
            task_gen.deploy_as(os.path.join('bld/packages', lua_dest, 'api', h.path_from(src)), h)
    if task_gen.env.STATIC:
        deploy_to(task_gen.link_task.outputs[0], 'lib.%s' % task_gen.env.VALID_ARCHITECTURES[0])
    else:
        if task_gen.env.DEST_BINFMT == 'pe':
            for file in task_gen.link_task.outputs[:-1]:
                deploy_to(file, 'bin.%s' % task_gen.env.VALID_ARCHITECTURES[0])
            deploy_to(task_gen.link_task.outputs[-1], 'lib.%s' % task_gen.env.VALID_ARCHITECTURES[0])
        else:
            for file in task_gen.link_task.outputs:
                deploy_to(file, 'bin.%s' % task_gen.env.VALID_ARCHITECTURES[0])


def build_source(bld, name, env, path):
    dll_flags = 'windows' in bld.env.VALID_PLATFORMS and ['LUA_BUILD_AS_DLL'] or []
    if bld.env.STATIC:
        return bld.static_library(
            name,
            env=env,
            path=path,
            extra_defines=['LUA_LIB', 'lua_getlocaledecpoint()=\'.\''],
            extra_includes=[path.make_node('src')],
            extra_public_includes=[path.make_node('src')],
            features=[
                'bugengine:masterfiles:off', 'bugengine:deploy:off', 'bugengine:deploy:lua', 'bugengine:warnings:off'
            ],
            source_list=LUA_SOURCES
        )

    else:
        return bld.shared_library(
            name,
            env=env,
            path=path,
            extra_defines=['LUA_LIB', 'lua_getlocaledecpoint()=\'.\''] + dll_flags,
            extra_includes=[path.make_node('src')],
            extra_public_includes=[path.make_node('src')],
            features=[
                'bugengine:masterfiles:off', 'bugengine:export_all', 'bugengine:deploy:off', 'bugengine:deploy:lua',
                'bugengine:warnings:off'
            ],
            source_list=LUA_SOURCES
        )


def build_binary(bld, name, env, path):
    return bld.thirdparty(name, source_node=path, env=env, use=bld.platforms)


def build(bld):
    bld.package('bugengine.3rdparty.scripting.lua', 'LUA_BINARY', build_binary, 'LUA_SOURCE', build_source)
