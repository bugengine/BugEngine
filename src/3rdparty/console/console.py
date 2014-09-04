def options(opt):
    pass

def setup(conf):
    pass

def build(bld):
    for env in bld.multiarch_envs:
        env['CFLAGS_3rdparty.console'] = env.CFLAGS_console
        env['CXXFLAGS_3rdparty.console'] = env.CXXFLAGS_console
        env['LINKFLAGS_3rdparty.console'] = env.LINKFLAGS_console
