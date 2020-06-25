def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option(
        '--with-freetype-source',
        action='store_true',
        default=False,
        dest='freetype_src',
        help='Compile the Freetype library from source (even if binaries could be downloaded)'
    )
    gr.add_option(
        '--deploy-freetype-package',
        action='store_true',
        default=False,
        dest='freetype_pkg',
        help='When building Freetype, deploy the binaries to a new package'
    )