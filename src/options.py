import os


def options(option_context):
    gr = option_context.get_option_group('build and install options')
    gr.add_option('--tests', action='store_true', default=True, dest='tests', help='enable tests')
    gr.add_option('--no-tests', action='store_false', default=True, dest='tests', help='disable tests')
    gr.add_option(
        '--fail-on-tests',
        action='store_true',
        default=True,
        dest='fail_on_unittest',
        help='fail the build if unit tests detect errors'
    )
    gr.add_option(
        '--no-fail-on-tests',
        action='store_false',
        default=True,
        dest='fail_on_unittest',
        help='Don\'t fail the build if unit tests detect errors'
    )

    third_party_node = option_context.path.make_node('bugengine/3rdparty')
    for category in third_party_node.listdir():
        if category[0] != '.':
            category_node = third_party_node.make_node(category)
            for third_party in category_node.listdir():
                option_context.recurse(
                    os.path.join(
                        third_party_node.path_from(option_context.path), category, third_party, 'mak', 'options.py'
                    )
                )
