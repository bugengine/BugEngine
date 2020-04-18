from be_typing import TYPE_CHECKING


def options(option_context):
    # type: (Options.OptionsContext) -> None
    option_context.recurse('../../DirectX/mak/options.py')


if TYPE_CHECKING:
    from waflib import Options

