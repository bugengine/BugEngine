import os


def options(option_context):
    gr = option_context.get_option_group('build and install options')
    gr.add_option('--tests', action='store_true', default=False, dest='tests', help='build tests')

    third_party_node = option_context.path.make_node('3rdparty')
    for category in third_party_node.listdir():
        if category[0] != '.':
            category_node = third_party_node.make_node(category)
            for third_party in category_node.listdir():
                option_context.recurse(
                    os.path.join(
                        third_party_node.path_from(option_context.path), category, third_party, 'mak', 'options.py'
                    )
                )
