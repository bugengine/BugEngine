def configure(configuration_context):
    configuration_context.load('clc', tooldir=[configuration_context.path.make_node('mak').abspath()])


def setup(configuration_context):
    if configuration_context.env.CLC_CXX:
        configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('opencl', configuration_context.env.TOOLCHAIN)])
