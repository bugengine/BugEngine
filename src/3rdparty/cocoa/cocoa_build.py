def build(bld):
    for env in bld.multiarch_envs:
        bld.thirdparty('3rdparty.cocoa', '.', env, frameworks=env.FRAMEWORK_COCOA)
        env.append_unique('FEATURES', ['GUI'])

