def configure(configuration_context):
    configuration_context.load('clc', tooldir=[configuration_context.path.make_node('tools').abspath()])