def build(bld):
    for env in bld.multiarch_envs:
        env['CFLAGS_3rdparty.system.console'] = env.CFLAGS_console
        env['CXXFLAGS_3rdparty.system.console'] = env.CXXFLAGS_console
        env['DEFINES_3rdparty.system.console'] = env.DEFINES_console
        env['LINKFLAGS_3rdparty.system.console'] = env.LINKFLAGS_console
