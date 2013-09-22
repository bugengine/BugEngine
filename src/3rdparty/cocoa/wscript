
def options(opt):
	pass

def setup(conf):
	if 'darwin' in conf.env.VALID_PLATFORMS:
		conf.check_framework(['Cocoa'], var='FRAMEWORK_COCOA')

def build(bld):
	for env in bld.multiarch_envs:
		bld.thirdparty('3rdparty.cocoa', env, frameworks=env.FRAMEWORK_COCOA)
		env.append_unique('FEATURES', ['GUI'])

