def build(bld):
    for env in bld.multiarch_envs:
        if env.OpenCL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenCL', '.',
                env,
                libs=env.OpenCL,
                includes=[bld.path.find_node('api')])
            env.append_unique('FEATURES', ['OpenCL'])

