def build_binary(bld, name, env, path):
    return bld.thirdparty(name, source_node=path, env=env)


def build(bld):
    bld.package('3rdparty.android.libc++', 'LIBCPP_BINARY', build_binary, 'LIBCPP_SOURCE', None)
