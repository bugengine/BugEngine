from be_typing import TYPE_CHECKING


def options(option_context):
    # type: (Options.OptionsContext) -> None
    option_context.add_package_options('gtk3')


if TYPE_CHECKING:
    from waflib import Options