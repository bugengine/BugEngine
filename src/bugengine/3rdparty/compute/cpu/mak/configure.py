def configure(configuration_context):
    configuration_context.load('kernel_cpu', tooldir=[configuration_context.path.make_node('tools').abspath()])
