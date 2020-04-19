def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework(['Cocoa'])


