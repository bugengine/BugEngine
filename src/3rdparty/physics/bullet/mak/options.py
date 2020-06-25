def options(opt):
    gr = opt.get_option_group('3rd party libraries')
    gr.add_option(
        '--with-bullet-source',
        action='store_true',
        default=False,
        dest='bullet_src',
        help='Compile the bullet library from source (even if binaries could be downloaded)'
    )
    gr.add_option(
        '--deploy-bullet-package',
        action='store_true',
        default=False,
        dest='bullet_pkg',
        help='When building bullet, deploy the binaries to a new package'
    )