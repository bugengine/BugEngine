import os

def build(bld):
	from waflib import Options
	for env in bld.multiarch_envs:
		includes=[os.path.join(Options.options.dx_sdk, 'Include')]
		libdirs=[os.path.join(Options.options.dx_sdk, 'Lib', 'x86'), os.path.join(Options.options.dx_sdk, 'Lib', 'x64'), os.path.join(Options.options.dx_sdk, 'Lib', 'arm')]
		if env.dx9 or bld.env.PROJECTS:
			bld.thirdparty('3rdparty.DirectX9', env, libs=env.dx9, lib_paths=libdirs, includes=includes)
			env.append_unique('FEATURES', ['DirectX9'])
		if env.dx10 or bld.env.PROJECTS:
			bld.thirdparty('3rdparty.DirectX10', env, libs=env.dx10, lib_paths=libdirs, includes=includes)
			env.append_unique('FEATURES', ['DirectX10'])
		if env.dx11 or bld.env.PROJECTS:
			bld.thirdparty('3rdparty.DirectX11', env, libs=env.dx11, lib_paths=libdirs, includes=includes)
			env.append_unique('FEATURES', ['DirectX11'])
