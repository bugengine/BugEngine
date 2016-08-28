def build(bld):
    for env in bld.multiarch_envs:
        if env.PSAPI or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.dbghelp', '.', env, libs=env.PSAPI)
