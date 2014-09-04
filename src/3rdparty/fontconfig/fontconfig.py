def options(opt):
	pass

def setup(conf):
	if 'posix' in conf.env.VALID_PLATFORMS:
		try:
			conf.pkg_config('fontconfig', var='3rdparty.fontconfig')
		except Exception as e:
			pass
		else:
			conf.env.SYSTEM_FONTCONFIG = True
