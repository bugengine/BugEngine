from be_typing import TYPE_CHECKING

from waflib import Options
from waflib.TaskGen import feature, after_method
import os


@feature('bugengine:deploy:gtk3')
@after_method('install_step')
@after_method('apply_link')
def deploy_gtk3_package(task_gen):
    if task_gen.env.PROJECTS:
        return

    path = task_gen.source_nodes[0]
    gtk3_dest = 'gtk3-3.24.22-%s-multiarch' % (task_gen.env.VALID_PLATFORMS[0])
    src = path.make_node('src')

    def deploy_to(file, subdir):
        if task_gen.bld.__class__.optim == 'debug':
            task_gen.deploy_as(
                os.path.join('bld/packages', gtk3_dest, subdir, task_gen.bld.__class__.optim, file.name), file
            )
        else:
            task_gen.deploy_as(os.path.join('bld/packages', gtk3_dest, subdir, file.name), file)

    if task_gen.env.TOOLCHAIN == task_gen.bld.multiarch_envs[0].TOOLCHAIN:
        for h in src.ant_glob(
            ['src/**/*.h']
        ):
            task_gen.deploy_as(os.path.join('bld', 'packages', gtk3_dest, 'api', h.path_from(src)), h)
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
    return None


def build_binary(bld, name, env, path):
    return bld.thirdparty(name, source_node=path, env=env, use=bld.platforms)


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.package('3rdparty.gui.gtk3', 'GTK3_BINARY', build_binary, 'GTK3_SOURCE', build_source)


if TYPE_CHECKING:
    from waflib import Build