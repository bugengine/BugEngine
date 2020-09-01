from be_typing import TYPE_CHECKING


def build(bld):
    # type: (Build.BuildContext) -> None
    bld.recurse('../libiconv/build.py')
    bld.recurse('../pcre/build.py')


if TYPE_CHECKING:
    from waflib import Build
