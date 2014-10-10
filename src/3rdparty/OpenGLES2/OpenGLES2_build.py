def build(bld):
    for env in bld.multiarch_envs:
        if env.GLESFramework or env.GLES2 or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenGLES2', '.', env, libs=env.GLES2+env.EGL, frameworks=env.GLESFramework)
            env.append_unique('FEATURES', ['OpenGLES2'])

