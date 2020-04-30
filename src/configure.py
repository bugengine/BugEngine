def configure(configuration_context):
    third_party_node = configuration_context.path.make_node('3rdparty')
    for category in third_party_node.listdir():
        if category[0] != '.':
            category_node = third_party_node.make_node(category)
            for third_party in category_node.listdir():
                configuration_context.recurse(
                    '%s/%s/%s/mak/configure.py' % (third_party_node.abspath(), category, third_party)
                )
