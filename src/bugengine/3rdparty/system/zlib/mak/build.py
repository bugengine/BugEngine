from waflib import Options
from waflib.TaskGen import feature, after_method
import os

ZLIB_SOURCE_LIST = [
    'adler32.c',
    'compress.c',
    'crc32.c',
    'deflate.c',
    'gzclose.c',
    'gzlib.c',
    'gzread.c',
    'gzwrite.c',
    'infback.c',
    'inffast.c',
    'inflate.c',
    'inftrees.c',
    'trees.c',
    'uncompr.c',
    'zutil.c',
]
MINIZIP_SOURCE_LIST = ['contrib/minizip/ioapi.c', 'contrib/minizip/unzip.c']


@feature('bugengine:deploy:zlib')
@after_method('install_step')
@after_method('apply_link')
def deploy_zlib_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    zlib_dest = 'zlib-1.2.11-%s-multiarch-%s' % (task_gen.env.VALID_PLATFORMS[0], task_gen.env.COMPILER_ABI)

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld', 'packages', zlib_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld', 'packages', zlib_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        task_gen.deploy_as(os.path.join('bld', 'packages', zlib_dest, 'api', 'zconf.h'), path.find_node('zconf.h'))
        task_gen.deploy_as(os.path.join('bld', 'packages', zlib_dest, 'api', 'zlib.h'), path.find_node('zlib.h'))
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


@feature('bugengine:deploy:minizip')
@after_method('install_step')
@after_method('apply_link')
def deploy_minizip_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    minizip_dest = 'minizip-1.2.11-%s-multiarch-%s' % (task_gen.env.VALID_PLATFORMS[0], task_gen.env.COMPILER_ABI)

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld', 'packages', minizip_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld', 'packages', minizip_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        task_gen.deploy_as(
            os.path.join('bld', 'packages', minizip_dest, 'api', 'ioapi.h'), path.find_node('contrib/minizip/ioapi.h')
        )
        task_gen.deploy_as(
            os.path.join('bld', 'packages', minizip_dest, 'api', 'unzip.h'), path.find_node('contrib/minizip/unzip.h')
        )
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


def build_zlib_source(bld, name, env, path):
    if bld.env.STATIC:
        return bld.static_library(
            name, ['bugengine.3rdparty.system.win32'],
            env=env,
            path=path,
            extra_includes=[path],
            extra_public_includes=[path],
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:deploy:off', 'bugengine:deploy:zlib'
            ],
            source_list=ZLIB_SOURCE_LIST
        )
    else:
        if 'windows' in bld.env.VALID_PLATFORMS and not bld.env.DISABLE_DLLEXPORT:
            dll_flags = ['ZLIB_DLL']
            dll_features = []
        else:
            dll_flags = []
            dll_features = ['bugengine:export_all']
        return bld.shared_library(
            name, ['bugengine.3rdparty.system.win32'],
            env=env,
            path=path,
            extra_includes=[path],
            extra_public_includes=[path],
            extra_defines=['Z_HAVE_UNISTD_H'] + dll_flags,
            extra_public_defines=dll_flags,
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:deploy:off', 'bugengine:deploy:zlib'
            ] + dll_features,
            source_list=ZLIB_SOURCE_LIST
        )


def build_zlib_binary(bld, name, env, path):
    result = bld.thirdparty(name, source_node=path, env=env)
    if not bld.env.STATIC:
        if 'windows' in bld.env.VALID_PLATFORMS and not bld.env.DISABLE_DLLEXPORT:
            result.export_defines.append('ZLIB_DLL')
    return result


def build_minizip_source(bld, name, env, path):
    include_path = path.make_node('contrib/minizip')
    if bld.env.STATIC:
        return bld.static_library(
            name, ['bugengine.3rdparty.system.zlib', 'bugengine.3rdparty.system.win32'],
            env=env,
            path=path,
            extra_includes=[include_path],
            extra_public_includes=[include_path],
            extra_defines=['USE_FILE32API', 'ZLIB_INTERNAL'],
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:deploy:off',
                'bugengine:deploy:minizip'
            ],
            source_list=MINIZIP_SOURCE_LIST
        )
    else:
        if 'windows' not in bld.env.VALID_PLATFORMS:
            features = ['bugengine:export_all']
        else:
            features = []
        return bld.shared_library(
            name, ['bugengine.3rdparty.system.zlib', 'bugengine.3rdparty.system.win32'],
            env=env,
            path=path,
            extra_includes=[include_path],
            extra_public_includes=[include_path],
            extra_defines=['USE_FILE32API', 'ZLIB_INTERNAL'],
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:deploy:off',
                'bugengine:deploy:minizip'
            ] + features,
            source_list=MINIZIP_SOURCE_LIST
        )


def build_minizip_binary(bld, name, env, path):
    return bld.thirdparty(name, var='minizip', source_node=path, use=['bugengine.3rdparty.system.zlib'], env=env)


def build(bld):
    bld.package('bugengine.3rdparty.system.zlib', 'ZLIB_BINARY', build_zlib_binary, 'ZLIB_SOURCE', build_zlib_source)
    bld.package(
        'bugengine.3rdparty.system.minizip', 'MINIZIP_BINARY', build_minizip_binary, 'MINIZIP_SOURCE',
        build_minizip_source
    )
