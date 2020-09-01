from be_typing import TYPE_CHECKING

from waflib import Options
from waflib.TaskGen import feature, after_method
import os


@feature('bugengine:deploy:pcre')
@after_method('install_step')
@after_method('apply_link')
def deploy_pcrepackage(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    pcre_dest = 'gtk3-%s-multiarch' % (task_gen.env.VALID_PLATFORMS[0])
    src = path.make_node('src')

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld/packages', pcre_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld/packages', pcre_dest, subdir, file.name), file)

    #if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
    #    for h in src.ant_glob(['src/**/*.h']):
    #        task_gen.deploy_as(os.path.join('bld', 'packages', libiconv_dest, 'api', h.path_from(src)), h)
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


_PCRE_SOURCES = [
    'pcre_byte_order.c',
    'pcre_compile.c',
    'pcre_config.c',
    'pcre_dfa_exec.c',
    'pcre_exec.c',
    'pcre_fullinfo.c',
    'pcre_get.c',
    'pcre_globals.c',
    'pcre_jit_compile.c',
    'pcre_maketables.c',
    'pcre_newline.c',
    'pcre_ord2utf8.c',
    'pcre_refcount.c',
    'pcre_string_utils.c',
    'pcre_study.c',
    'pcre_tables.c',
    'pcre_ucd.c',
    'pcre_valid_utf8.c',
    'pcre_version.c',
    'pcre_xclass.c',
]


def _build_source(bld, name, env, path):
    if bld.env.STATIC:
        result = bld.static_library(
            name,
            env=env,
            depends=bld.platforms + ['bugengine.config'],
            path=path,
            extra_defines=['HAVE_CONFIG_H', 'PCRE_STATIC'],
            extra_public_defines=['PCRE_STATIC'],
            extra_includes=[
                bld.path.make_node('api').abspath(),
                bld.path.make_node('include').abspath(),
                path.abspath()
            ],
            extra_public_includes=[
                bld.path.make_node('api').abspath(),
            ],
            features=[
                'bugengine:masterfiles:off', 'bugengine:deploy:off', 'bugengine:deploy:pcre', 'bugengine:warnings:off'
            ],
            source_list=_PCRE_SOURCES
        )
        result.source += bld.path.ant_glob('src/**')
        return result
    else:
        defines = ['HAVE_CONFIG_H']
        if 'windows' in bld.env.VALID_PLATFORMS and not bld.env.DISABLE_DLLEXPORT:
            dll_features = []
        elif bld.env.DISABLE_DLLEXPORT:
            dll_features = ['bugengine:export_all']
        else:
            dll_features = []
        result = bld.shared_library(
            name,
            env=env,
            depends=bld.platforms + ['bugengine.config'],
            path=path,
            extra_defines=defines,
            extra_includes=[
                bld.path.make_node('api').abspath(),
                bld.path.make_node('include').abspath(),
                path.abspath()
            ],
            extra_public_includes=[
                bld.path.make_node('api').abspath(),
            ],
            features=[
                'bugengine:masterfiles:off', 'bugengine:deploy:off', 'bugengine:deploy:pcre', 'bugengine:warnings:off'
            ] + dll_features,
            source_list=_PCRE_SOURCES
        )
        result.source += bld.path.ant_glob('src/**')
        return result


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.package('bugengine.3rdparty.gui.pcre', 'PCRE_BINARY', None, 'PCRE_SOURCE', _build_source)


if TYPE_CHECKING:
    from waflib import Build
