import os


def setup(conf):
    if not conf.env.PROJECTS:
        conf.start_msg_setup()
        if 'darwin' in conf.env.VALID_PLATFORMS:
            if conf.check_framework('OpenGL', includepath=[os.path.join(conf.path.parent.abspath(), 'api')]):
                conf.end_msg('from system')
            else:
                conf.end_msg('not found', color='YELLOW')
        elif 'windows' in conf.env.VALID_PLATFORMS:
            if conf.check_lib(
                ['opengl32', 'gdi32'],
                includes=['windows.h', 'GL/gl.h'],
                includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                functions=['glBegin']
            ):
                conf.end_msg('from system')
            else:
                conf.end_msg('not found', color='YELLOW')
        else:
            try:
                conf.pkg_config('gl', var='OpenGL')
                conf.env.append_unique('check_OpenGL_includes', [os.path.join(conf.path.parent.abspath(), 'api')])
            except Exception as e:
                if conf.check_lib(
                    'GL',
                    var='OpenGL',
                    includes=['GL/gl.h'],
                    includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                    functions=['glBegin']
                ):
                    conf.end_msg('from system')
                else:
                    conf.end_msg('not found', color='YELLOW')
            else:
                conf.env.SYSTEM_OPENGL = True
                conf.end_msg('from system')
