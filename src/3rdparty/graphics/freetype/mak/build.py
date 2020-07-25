from waflib import Options
from waflib.TaskGen import feature, after_method
import os

FT_SOURCE_LIST = [
    'src/autofit/autofit.c',
    'src/base/ftbase.c',
    'src/base/ftbbox.c',
    'src/base/ftbdf.c',
    'src/base/ftbitmap.c',
    'src/base/ftcid.c',
    'src/base/ftdebug.c',
    'src/base/ftfstype.c',
    'src/base/ftgasp.c',
    'src/base/ftglyph.c',
    'src/base/ftgxval.c',
    'src/base/ftinit.c',
    'src/base/ftmm.c',
    'src/base/ftotval.c',
    'src/base/ftpatent.c',
    'src/base/ftpfr.c',
    'src/base/ftstroke.c',
    'src/base/ftsynth.c',
    'src/base/ftsystem.c',
    'src/base/fttype1.c',
    'src/base/ftwinfnt.c',
    'src/bdf/bdf.c',
    'src/bzip2/ftbzip2.c',
                                   #'src/cache/ftcache.c',
    'src/cache/ftcbasic.c',
    'src/cache/ftccache.c',
    'src/cache/ftccmap.c',
    'src/cache/ftcglyph.c',
    'src/cache/ftcimage.c',
    'src/cache/ftcmanag.c',
    'src/cache/ftcmru.c',
    'src/cache/ftcsbits.c',
    'src/cff/cff.c',
    'src/cid/type1cid.c',
    'src/gxvalid/gxvalid.c',
    'src/gzip/ftgzip.c',
    'src/lzw/ftlzw.c',
    'src/otvalid/otvalid.c',
    'src/pcf/pcf.c',
    'src/pfr/pfr.c',
    'src/psaux/psaux.c',
    'src/pshinter/pshinter.c',
    'src/psnames/psnames.c',
    'src/raster/raster.c',
    'src/sfnt/sfnt.c',
    'src/smooth/smooth.c',
    'src/truetype/truetype.c',
    'src/type1/type1.c',
    'src/type42/type42.c',
    'src/winfonts/winfnt.c',
]


@feature('bugengine:freetype:deploy')
@after_method('install_step')
@after_method('apply_link')
def deploy_freetype_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    ft_dest = 'freetype-2.10.2-%s-multiarch-%s' % (task_gen.env.VALID_PLATFORMS[0], task_gen.env.COMPILER_ABI)

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld', 'packages', ft_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld', 'packages', ft_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        include = path.make_node('include')
        for h in include.ant_glob(['**/*.*']):
            task_gen.deploy_as(os.path.join('bld', 'packages', ft_dest, 'api', h.path_from(include)), h)
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
    defines = ['FT2_BUILD_LIBRARY=1', 'FT_CONFIG_OPTION_SYSTEM_ZLIB=1', 'FT_CONFIG_OPTION_NO_ASSEMBLER']
    include_path = path.make_node('include')
    includes = []
    for i in include_path.listdir():
        includes.append(include_path.find_node(i))
    if bld.env.STATIC:
        return bld.static_library(
            name, ['3rdparty.system.zlib'],
            env=env,
            path=path,
            extra_includes=[include_path],
            extra_public_includes=[include_path],
            extra_defines=defines,
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:freetype:deploy', 'bugengine:nortc'
            ],
            source_list=FT_SOURCE_LIST
        )
    else:
        return bld.shared_library(
            name, ['3rdparty.system.zlib'],
            env=env,
            path=path,
            extra_includes=[include_path],
            extra_public_includes=[include_path],
            extra_defines=defines + ['DLL_EXPORT'],
            features=[
                'bugengine:masterfiles:off', 'bugengine:warnings:off', 'bugengine:freetype:deploy', 'bugengine:nortc'
            ],
            source_list=FT_SOURCE_LIST
        )


def build_binary(bld, name, env, path):
    return bld.thirdparty(name, source_node=path, use=['3rdparty.system.zlib'], env=env)


def build(bld):
    bld.package('3rdparty.graphics.freetype', 'FREETYPE_BINARY', build_binary, 'FREETYPE_SOURCE', build_source)
