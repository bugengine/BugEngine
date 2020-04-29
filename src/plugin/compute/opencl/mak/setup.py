def setup(configuration_context):
    if configuration_context.env.check_OpenCL:
        configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('opencl', configuration_context.env.TOOLCHAIN)])
        configuration_context.env.append_value(
            'CLC_CXXFLAGS', ['-I%s' % configuration_context.path.parent.make_node('api.cl').abspath()]
        )
