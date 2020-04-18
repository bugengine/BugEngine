from be_typing import TYPE_CHECKING


def setup(confifuration_context):
    # type: (Configure.ConfigurationContext) -> None
    confifuration_context.recurse('../../DirectX/mak/setup.py')


if TYPE_CHECKING:
    from waflib import Configure