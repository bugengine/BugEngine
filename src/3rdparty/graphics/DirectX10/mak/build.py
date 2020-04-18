from be_typing import TYPE_CHECKING


def build(build_context):
    # type: (Build.BuildContext) -> None
    build_context.recurse('../../DirectX/mak/build.py')


if TYPE_CHECKING:
    from waflib import Build