from waflib.Errors import WafError


def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        try:
            conf.pkg_config('x11', var='X11')
            conf.end_msg('from pkg-config')
        except WafError:
            if conf.check_lib(['X11'], includepath=['/usr/X11R6/include']):
                conf.end_msg('from system')
            else:
                conf.end_msg('not found', color='RED')
