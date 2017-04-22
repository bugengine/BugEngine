
def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL')
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['opengl32', 'gdi32'],
                       includes=['GL/gl.h'],
                       functions=['glBegin'])
    else:
        conf.check_lib('GL',
                       includes=['GL/gl.h'],
                       functions=['glBegin'])
