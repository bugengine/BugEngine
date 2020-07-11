from waflib import Options
from waflib.TaskGen import feature, after_method
import os


@feature('bugengine:bullet:deploy')
@after_method('install_step')
@after_method('apply_link')
def deploy_bullet_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    bullet_dest = 'bullet-2.87-%s-multiarch-%s' % (task_gen.env.VALID_PLATFORMS[0], task_gen.env.COMPILER_ABI)

    src = path.make_node('src')
    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        for h in src.ant_glob(
            ['*.h', 'BulletCollision/**/*.h', 'BulletDynamics/**/*.h', 'BulletSoftBody/**/*.h', 'LinearMath/**/*.h']
        ):
            task_gen.deploy_as(os.path.join('packages', bullet_dest, 'api', h.path_from(src)), h)
    lib_file = task_gen.link_task.outputs[0]
    if task_gen.bld.__class__.optim == 'debug':
        task_gen.deploy_as(
            os.path.join(
                'packages', bullet_dest, 'lib.%s' % task_gen.env.VALID_ARCHITECTURES[0], task_gen.bld.__class__.optim,
                lib_file.name
            ), lib_file
        )
    else:
        task_gen.deploy_as(
            os.path.join('packages', bullet_dest, 'lib.%s' % task_gen.env.VALID_ARCHITECTURES[0], lib_file.name),
            lib_file
        )


def build_source(bld, name, env, path):
    return bld.static_library(
        name,
        bld.platforms,
        path=path,
        features=['bugengine:warnings:off', 'bugengine:bullet:deploy', 'bugengine:nortc'],
        extra_includes=[path.make_node('src')],
        extra_public_includes=[path.make_node('src')],
        extra_defines=[
            '_ALLOW_MSC_VER_MISMATCH=1', '_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH=1', '_ALLOW_RUNTIME_LIBRARY_MISMATCH=1',
            'BT_NO_PROFILE'
        ],
        env=env,
        source_list=[
            'src/BulletCollision/**/*.cpp', 'src/BulletDynamics/**/*.cpp', 'src/BulletSoftBody/**/*.cpp',
            'src/LinearMath/**/*.cpp'
        ]
    )


def build_binary(bld, name, env, path):
    return bld.thirdparty(name, source_node=path, env=env, use=bld.platforms)


def build(bld):
    bld.package('3rdparty.physics.bullet', 'BULLET_BINARY', build_binary, 'BULLET_SOURCE', build_source)
