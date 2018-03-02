from waflib import Options, Errors


def deploy(build_context):
    build_context.load_device_list()
    device_name = Options.options.device
    for device in build_context.devices:
        if device.name == device_name:
            status = device.get_status()
            if status.connection_status != 'online':
                raise Errors.WafError('Device "%s" is currently in %s state (%s)' % (device_name,
                                                                                     status.connection_status,
                                                                                     status.message))
            if status.platform != 'Android':
                raise Errors.WafError('Expected Android device; "%s" is %s' % (device_name, status.platform))
            print('deploy')
            break
    else:
        raise Errors.WafError('Unknown device "%s"' % device_name)

