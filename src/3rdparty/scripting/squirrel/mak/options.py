def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option('--with-included-squirrel',
                  action='store_true',
                  default=False,
                  dest='included_squirrel',
                  help='Compile the squirrel library from the included code')
