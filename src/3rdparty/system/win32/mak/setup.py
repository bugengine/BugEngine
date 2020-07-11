import os


def setup(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        if conf.check_lib(
            ['kernel32', 'user32', 'advapi32', 'ole32', 'oleaut32', 'uuid', 'shell32'],
            includepath=[os.path.join(conf.path.parent.abspath(), 'api.windows')]
        ):
            conf.end_msg('from system')
        else:
            conf.end_msg('not found', color='RED')
