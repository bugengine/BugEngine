def options(opt):
    pass

def setup(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['kernel32', 'user32', 'advapi32', 'ole32', 'oleaut32', 'uuid', 'shell32'],
                       includepath=[os.path.join(conf.path.abspath(), 'api.windows'))
