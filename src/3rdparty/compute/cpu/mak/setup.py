from waflib.Logs import pprint


def setup(configuration_context):
    pprint('GREEN', 'cpu[%s]'%','.join(['vanilla']+configuration_context.env.VECTOR_OPTIM_VARIANTS), sep=' ')
    configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cpu', configuration_context.env.TOOLCHAIN)])