def options(opt):
	pass

def setup(conf):
	if conf.env.ANDROID_SDK >= 16:
		conf.env.OUYA_JAR = conf.path.make_node('lib').make_node('ouya-sdk.jar').abspath()

def build(bld):
	if bld.env.OUYA_JAR:
		bld.thirdparty('ouya', bld.env)
		bld.features.add('ouya')
		bld.env.append_unique('CLASSPATH_ouya', [conf.env.OUYA_JAR])
