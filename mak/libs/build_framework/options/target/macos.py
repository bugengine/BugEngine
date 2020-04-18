from be_typing import TYPE_CHECKING


def options(opt):
    # type: (Options.OptionsContext) -> None
    gr = opt.get_option_group('SDK paths and options')
    gr.add_option('--macosx-sdk-min',
                  action='store',
                  default='',
                  dest='macosx_sdk_min',
                  help='Minimum version of the MacOS X SDK to target')
    gr.add_option('--macosx-sdk-max',
                  action='store',
                  default='',
                  dest='macosx_sdk_max',
                  help='Maximum version of the MacOS X SDK to target')


if TYPE_CHECKING:
    from waflib import Options
