import os

def options(opt):
	pass

def setup(conf):
	conf.check_lib(['psapi', 'dbghelp', 'version'], var='PSAPI', libpath=[os.path.join(conf.path.abspath(), 'lib.%s.%s'%(conf.env.VALID_PLATFORMS[0], a)) for a in conf.env.VALID_ARCHITECTURES])

def build(bld):
	for env in bld.multiarch_envs:
		if env.PSAPI:
			bld.thirdparty('3rdparty.dbghelp', env, libs=env.PSAPI)
