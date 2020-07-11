import os


def setup(conf):
    if not conf.env.PROJECTS:
        conf.start_msg_setup()
        if 'darwin' in conf.env.VALID_PLATFORMS:
            if conf.check_framework('OpenGLES'):
                conf.end_msg('from system')
            else:
                conf.end_msg('not found', color='YELLOW')
        else:
            try:
                conf.pkg_config('glesv2', var='OpenGLES2')
                conf.pkg_config('egl', var='OpenGLES2')
            except Exception as e:
                if conf.check_lib(
                    ['GLESv2', 'EGL'],
                    var='OpenGLES2',
                    includepath=[os.path.join(conf.path.parent.abspath(), 'api')],
                    includes=['GLES2/gl2.h', 'EGL/egl.h'],
                    functions=['glGenFramebuffers', 'eglCreateContext']
                ):
                    conf.end_msg('from system')
                else:
                    conf.end_msg('not found', color='YELLOW')
            else:
                conf.env.SYSTEM_OPENGLES2 = True
                conf.end_msg('from system')
