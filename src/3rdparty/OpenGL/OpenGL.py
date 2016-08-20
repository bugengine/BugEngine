
def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL', var='GLFramework')
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['opengl32', 'gdi32'], var='GL',
                       includes=['GL/gl.h'],
                       functions=['glBegin(GL_TRIANGLES)'])
    else:
        conf.check_lib('GL', var='GL',
                       includes=['GL/gl.h'],
                       functions=['glBegin(GL_TRIANGLES)'])
