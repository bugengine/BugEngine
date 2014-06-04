
def options(opt):
    pass

def setup(conf):
    if 'darwin' in conf.env.VALID_PLATFORMS:
        conf.check_framework('OpenGL', var='GLFramework')
    else:
        conf.check_lib(['OpenGL32', 'gdi32'], var='OpenGL') or conf.check_lib('GL', var='OpenGL')

def build(bld):
    for env in bld.multiarch_envs:
        if env.GLFramework or env.OpenGL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenGL', env, libs=env.OpenGL, frameworks=env.GLFramework)
            env.append_unique('FEATURES', ['OpenGL'])
