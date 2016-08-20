def build(bld):
    for env in bld.multiarch_envs:
        if env.GLFramework or env.GL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenGL', '.', env, libs=env.GL, frameworks=env.GLFramework)
            env.append_unique('FEATURES', ['OpenGL'])
