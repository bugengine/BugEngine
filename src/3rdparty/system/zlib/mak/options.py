def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option(
        '--with-zlib-source',
        action='store_true',
        default=False,
        dest='zlib_src',
        help='Compile the Zlib library from source (even if binaries could be downloaded)'
    )
    gr.add_option(
        '--deploy-zlib-package',
        action='store_true',
        default=False,
        dest='zlib_pkg',
        help='When building Zlib, deploy the binaries to a new package'
    )
    gr.add_option(
        '--with-minizip-source',
        action='store_true',
        default=False,
        dest='minizip_src',
        help='Compile the Minizip library from source (even if binaries could be downloaded)'
    )
    gr.add_option(
        '--deploy-minizip-package',
        action='store_true',
        default=False,
        dest='minizip_pkg',
        help='When building Minizip, deploy the binaries to a new package'
    )