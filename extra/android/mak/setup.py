def setup(configuration_context):
    configuration_context.start_msg('      `- [android]')
    category_node = configuration_context.path.make_node('src/3rdparty/android')
    for third_party in category_node.listdir():
        configuration_context.recurse('src/3rdparty/android/%s/%s.py' % (third_party, third_party),
                                      name='setup',
                                      once=False)
    configuration_context.end_msg(' ')