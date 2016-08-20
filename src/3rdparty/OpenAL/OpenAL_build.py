def build(bld):
    for env in bld.multiarch_envs:
        if env.ALFramework or  env.AL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenAL', '.', env, libs=env.AL, frameworks=env.ALFramework)
            env.append_unique('FEATURES', ['OpenAL'])
