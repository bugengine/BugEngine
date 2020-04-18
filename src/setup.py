def setup(configuration_context):
    third_party_node = configuration_context.path.make_node('3rdparty')
    for category in third_party_node.listdir():
        configuration_context.start_msg('  [%s]' % category)
        category_node = third_party_node.make_node(category)
        for third_party in category_node.listdir():
            configuration_context.recurse('%s/%s/%s/mak/setup.py' % (third_party_node.abspath(), category, third_party), once=False)
        configuration_context.end_msg(' ')