def build(bld):
    for env in bld.multiarch_envs:
        if env.GLFramework or env.OpenGL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenGL', env, libs=env.OpenGL, frameworks=env.GLFramework)
            env.append_unique('FEATURES', ['OpenGL'])
