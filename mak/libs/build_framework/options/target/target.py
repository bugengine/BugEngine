import os
from be_typing import TYPE_CHECKING


def options(option_context):
    # type: (Options.OptionsContext) -> None
    gr = option_context.add_option_group('configure options')
    gr.add_option('--platforms', action='store', default='', dest='platforms', help='List of platform to configure for')

    for target in os.listdir(option_context.path.abspath()):
        option_context.recurse(target) # conveniently will not recurse in this module again


if TYPE_CHECKING:
    from waflib import Options