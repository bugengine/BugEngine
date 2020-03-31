def configure(configuration_context):
    pass


def setup(configuration_context):
    configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cpu', configuration_context.env.TOOLCHAIN)])
