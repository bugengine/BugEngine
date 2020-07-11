def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        if conf.check_framework(['Cocoa']):
            conf.end_msg('from system')
        else:
            conf.end_msg('not found', color='RED')
