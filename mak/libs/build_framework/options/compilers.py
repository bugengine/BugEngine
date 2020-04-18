from be_typing import TYPE_CHECKING


def options(option_context):
    # type: (Options.OptionsContext) -> None
    gr = option_context.add_option_group('configure options')
    gr.add_option('--compilers',
                  action='store',
                  default='',
                  dest='compilers',
                  help='List of compilers to configure for')


if TYPE_CHECKING:
    from waflib import Options