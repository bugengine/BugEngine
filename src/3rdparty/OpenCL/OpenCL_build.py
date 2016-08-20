def build(bld):
    for env in bld.multiarch_envs:
        if env.CL or bld.env.PROJECTS:
            bld.thirdparty('3rdparty.OpenCL', '.',
                env,
                libs=env.CL,
                includes=[bld.path.find_node('api')])
            env.append_unique('FEATURES', ['OpenCL'])

