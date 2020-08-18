from be_typing import TYPE_CHECKING


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.recurse('glib/build.py')
    bld.recurse('gobject/build.py')
    bld.recurse('gtk/build.py')


if TYPE_CHECKING:
    from waflib import Build
