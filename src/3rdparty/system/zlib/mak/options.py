from be_typing import TYPE_CHECKING


def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-zlib',
                  action='store_true',
                  default=False,
                  dest='included_zlib',
                  help='Compile the zlib library from the included code')


if TYPE_CHECKING:
    from waflib import Options