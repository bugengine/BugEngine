def options(opt):
    gr = opt.add_option_group('configure options')
    gr.add_option(
        '--python-versions',
        action='store',
        dest='python_versions',
        help='List of Python version to support in plugins',
        default='2.6,2.7,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9'
    )
