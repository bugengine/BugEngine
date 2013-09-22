
def options(opt):
	pass

def setup(conf):
	conf.check_lib(['X11'])


def build(bld):
	for env in bld.multiarch_envs:
		if env.X11:
			bld.thirdparty('3rdparty.X11', env, includes=['/usr/X11R6/include'], libs=env.X11)
			env.append_unique('FEATURES', ['GUI'])
