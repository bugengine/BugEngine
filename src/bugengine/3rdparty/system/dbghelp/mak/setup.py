import os


def setup(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        if conf.check_lib(
            ['psapi', 'version'],
            includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
            includes=['windows.h', 'dbghelp.h'],
        ):
            conf.end_msg('from system')
        else:
            conf.end_msg('not found', color='RED')
