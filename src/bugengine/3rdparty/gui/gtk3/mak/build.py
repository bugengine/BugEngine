from be_typing import TYPE_CHECKING


def build_source(bld, name, env, path):
    # type: (Build.BuildContext, str, ConfigSet.ConfigSet, Node.Node) -> None
    bld.recurse('../gtk/build.py')


def build_binary(bld, name, env, path):
    # type: (Build.BuildContext, str, ConfigSet.ConfigSet, Node.Node) -> None
    return bld.thirdparty(name, var='gtk3', source_node=path, env=env, use=bld.platforms)


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.package('bugengine.3rdparty.gui.gtk3', 'GTK3_BINARY', build_binary, 'GTK3_SOURCE', build_source)


if TYPE_CHECKING:
    from waflib import Build, ConfigSet, Node
