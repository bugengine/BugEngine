from waflib.Configure import conf


@conf
def package(build_context, name, system_var, binary_build, source_var, source_build):
    arch_list = []
    preprocess = None
    for env in build_context.multiarch_envs:
        pkg_path = env[system_var]
        if not pkg_path or env.PROJECTS:
            try:
                path = build_context.package_node.make_node(env[source_var])
            except KeyError:
                assert build_context.env.PROJECTS
            else:
                if not preprocess:
                    preprocess = build_context.preprocess(name, path, '', name)
                arch_list.append(source_build(build_context, name, env, path))
        else:
            path = build_context.package_node.make_node(pkg_path)
            arch_list.append(binary_build(build_context, name, env, path))
    build_context.multiarch(name, arch_list)


def build(build_context):
    pass