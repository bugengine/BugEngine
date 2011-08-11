VERSION = "0.2.0"
APPNAME = "BugEngine"

from mak import module
import os
from waflib import Options, Logs

top = '.'
out = '.build/waf'

def options(opt):
	opt.recurse('mak')

def configure(conf):
	conf.recurse('mak')


def build(bld):
	if not bld.variant:
		bld.recurse('mak')
		if not bld.env.PROJECTS:
			Options.commands.extend(['build_' + i for i in bld.env.BUILD_VARIANTS])
			return

	dbghelp			= module.external('dbghelp')
	directx9		= module.external('DirectX9')
	directx10		= module.external('DirectX10')
	opengl			= module.external('OpenGL')
	cgDx			= module.external('CgDx')
	cgGL			= module.external('CgGL')
	X11				= module.external('X11')
	win32			= module.external('win32')

	freetype		= module.external('freetype')

	openal			= module.external('OpenAL')
	oggvorbis		= module.external('oggvorbis')

	bulletengine	= module.external('bulletengine')

	lualib			= module.external('lualib')
	squirellib		= module.external('squirrellib')

	mak				= module.external('mak')

	core			= module.library('core',        [dbghelp, win32, mak])
	network			= module.library('network',     [core])
	rtti			= module.library('rtti',        [core, network] )
	system			= module.library('system',      [core, rtti] )
	input			= module.library('input',       [core, rtti] )
	mobile			= module.library('mobile',      [core, rtti, system, input] )
	main			= module.library('main',        [core, rtti, system, input, mobile])

	discworld		= module.game('discworld',      [core, rtti, system, input, mobile, main])

	editor			= module.plugin('editor',       [discworld], platforms=['pc'])

	windowing		= module.library('windowing',   [discworld, X11, win32], category='plugin')
	bullet			= module.plugin('bullet',		[discworld, bulletengine])

	_3d				= module.plugin('3d', 			[discworld])
	shadermodel1	= module.plugin('shadermodel1', [discworld, _3d])
	shadermodel2	= module.plugin('shadermodel2', [discworld, _3d, shadermodel1])
	shadermodel3	= module.plugin('shadermodel3', [discworld, _3d, shadermodel1, shadermodel2])
	shadermodel4	= module.plugin('shadermodel4', [discworld, _3d, shadermodel1, shadermodel2, shadermodel3])

	windowing		= module.library('windowing',   [discworld, _3d, X11, win32], category='plugin')
	gl				= module.plugin('GL', 			[discworld, windowing, opengl, _3d, shadermodel1, shadermodel2, shadermodel3, shadermodel4], platforms=['pc'])
	Dx9				= module.plugin('DX9',          [discworld, windowing, cgDx, directx9, _3d, shadermodel1, shadermodel2], platforms=['win32'])
	#Dx10			= module.plugin('DX10',         [discworld, windowing, cgDx, directx10, shadermodel4], platforms=['win32'])
	nullrender		= module.plugin('nullrender', 	[discworld, _3d, shadermodel1, shadermodel2, shadermodel3, shadermodel4])
	#AL				= module.plugin('AL',  			[discworld, openal], platforms=['pc'])
	package			= module.plugin('package',      [discworld])
	lua				= module.plugin('lua',          [discworld, lualib])
	squirrel		= module.plugin('squirrel',     [discworld, squirellib])

	discworld.plugins=[_3d, shadermodel1, shadermodel2, shadermodel3, shadermodel4, gl, Dx9, nullrender, bullet, package, lua, squirrel]
	discworld.post(bld)



from waflib.Build import BuildContext, InstallContext, UninstallContext
from waflib import ConfigSet
try:
	env = ConfigSet.ConfigSet('.build/be_toolchains.py')
	for toolchain in env.BUILD_VARIANTS:
		for y in (BuildContext, InstallContext, UninstallContext):
			name = y.__name__.replace('Context','').lower()
			class tmp(y):
				cmd = name + '_' + toolchain
				variant = toolchain
except:
	pass

