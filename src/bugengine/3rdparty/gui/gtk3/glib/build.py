from be_typing import TYPE_CHECKING


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.recurse('../libiconv/build.py')


if TYPE_CHECKING:
    from waflib import Build
