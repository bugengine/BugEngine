from be_typing import TYPE_CHECKING

from waflib import Options
from waflib.TaskGen import feature, after_method
import os


@feature('bugengine:deploy:libiconv')
@after_method('install_step')
@after_method('apply_link')
def deploy_libiconv_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    libiconv_dest = 'gtk3-%s-multiarch' % (task_gen.env.VALID_PLATFORMS[0])
    src = path.make_node('src')

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld/packages', libiconv_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld/packages', libiconv_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        for h in src.ant_glob(['src/**/*.h']):
            task_gen.deploy_as(os.path.join('bld', 'packages', libiconv_dest, 'api', h.path_from(src)), h)
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
    defines = ['IN_LIBRARY', 'BUILDING_LIBICONV']

    source_list = [
        'lib/relocatable.c',
        'libcharset/lib/localcharset.c',
    ]
    if bld.env.CC_NAME != 'msvc':
        source_list.append('lib/iconv.c')
    if bld.env.STATIC:
        result = bld.static_library(
            name,
            env=env,
            depends=bld.platforms + ['bugengine.config'],
            path=path,
            extra_defines=defines,
            extra_includes=[
                path.make_node('lib').abspath(),
                bld.path.make_node('api').abspath(),
                bld.path.make_node('include').abspath()
            ],
            extra_public_includes=[bld.path.make_node('api').abspath()],
            features=[
                'bugengine:masterfiles:off', 'bugengine:deploy:off', 'bugengine:deploy:libiconv',
                'bugengine:warnings:off'
            ],
            source_list=source_list
        )
        if bld.env.CC_NAME == 'msvc':
            result.source.append(bld.path.find_node('src/libiconv.cc'))
        return result
    else:
        if 'windows' in bld.env.VALID_PLATFORMS and not bld.env.DISABLE_DLLEXPORT:
            dll_defines = ['BUILDING_DLL']
            dll_features = []
        elif bld.env.DISABLE_DLLEXPORT:
            dll_defines = []
            dll_features = ['bugengine:export_all']
        else:
            dll_defines = ['BUILDING_DLL']
            dll_features = []
        result = bld.shared_library(
            name,
            env=env,
            depends=bld.platforms + ['bugengine.config'],
            path=path,
            extra_defines=defines + dll_defines,
            extra_includes=[
                path.make_node('lib').abspath(),
                bld.path.make_node('api').abspath(),
                bld.path.make_node('include').abspath()
            ],
            extra_public_includes=[bld.path.make_node('api').abspath()],
            features=[
                'bugengine:masterfiles:off', 'bugengine:deploy:off', 'bugengine:deploy:libiconv',
                'bugengine:warnings:off'
            ] + dll_features,
            source_list=source_list
        )
        if bld.env.CC_NAME == 'msvc':
            result.source.append(bld.path.find_node('src/libiconv.cc'))
        return result


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.package('bugengine.3rdparty.gui.libiconv', 'LIBICONV_BINARY', None, 'LIBICONV_SOURCE', build_source)


if TYPE_CHECKING:
    from waflib import Build
