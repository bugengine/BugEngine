def configure(configuration_context):
    configuration_context.recurse(configuration_context.env.ARCH_NAME + '.py', once=False)
