
def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL', var='GLFramework')
    else:
        conf.check_lib(['OpenGL32', 'gdi32'], var='OpenGL') or conf.check_lib('GL', var='OpenGL')
