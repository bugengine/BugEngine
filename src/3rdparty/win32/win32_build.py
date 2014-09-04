def build(bld):
	bld.thirdparty('3rdparty.win32', bld.env, libs= ['kernel32', 'user32', 'advapi32',
							'ole32', 'oleaut32', 'uuid', 'shell32', ])
	bld.env.append_unique('FEATURES', ['GUI'])
