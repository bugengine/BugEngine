def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-minizip',
                  action='store_true',
                  default=False,
                  dest='included_minizip',
                  help='Compile the minizip library from the included code')
