import os


def setup(conf):
    if 'windows' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        if conf.check_lib(
            ['psapi', 'dbghelp', 'version'],
            includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
            libpath=[
                os.path.join(conf.path.parent.abspath(), 'lib.%s.%s' % (conf.env.VALID_PLATFORMS[0], a))
                for a in conf.env.VALID_ARCHITECTURES
            ],
            includes=['windows.h', 'dbghelp.h'],
            functions=['SymInitialize']
        ):
            conf.end_msg('from system')
        else:
            conf.end_msg('not found', color='RED')
