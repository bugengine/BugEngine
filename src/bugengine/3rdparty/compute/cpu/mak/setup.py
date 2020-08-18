def setup(configuration_context):
    if not configuration_context.env.PROJECTS:
        configuration_context.start_msg_setup()
        configuration_context.end_msg(
            ', '.join(['vanilla'] + configuration_context.env.VECTOR_OPTIM_VARIANTS), color='GREEN'
        )
        configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cpu', configuration_context.env.TOOLCHAIN)])