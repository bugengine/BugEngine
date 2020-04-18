def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-freetype',
                  action='store_true',
                  default=False,
                  dest='included_freetype',
                  help='Compile the Freetype library from the included code')
