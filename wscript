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

	bullet			= module.external('bullet')

	lualib			= module.external('lualib')
	squirellib		= module.external('squirrellib')
	angelcodelib	= module.external('angelcodelib')

	mak				= module.external('mak')

	core			= module.library('core',        [dbghelp, win32, mak])
	network			= module.library('network',     [core])
	rtti			= module.library('rtti',        [core, network] )
	system			= module.library('system',      [core, rtti] )
	input			= module.library('input',       [core, rtti] )
	graphics		= module.library('graphics',    [core, rtti, system, input, freetype ] )
	audio			= module.library('audio',       [core, rtti, system, oggvorbis] )
	physics			= module.library('physics',     [core, rtti, system] )
	mobile			= module.library('mobile',      [core, rtti, system, graphics, audio, physics, input] )
	main			= module.library('main',        [core, rtti, system, input, graphics, audio, physics, mobile])

	discworld		= module.game('discworld',      [core, rtti, system, input, graphics, audio, physics, mobile, main])

	editor			= module.plugin('editor',       [discworld], platforms=['pc'])
	windowing		= module.library('windowing',   [discworld, X11, win32], category='plugin')
	physicsBullet	= module.plugin('physicsBullet',[discworld, bullet])
	renderNull		= module.plugin('renderNull', 	[discworld])
	windowing		= module.library('windowing',   [discworld, X11, win32], category='plugin')
	renderOpenGL	= module.plugin('renderOpenGL', [discworld, windowing, opengl], platforms=['pc'])
	renderDx9		= module.plugin('renderDx9',    [discworld, windowing, cgDx, directx9], platforms=['win32'])
	#audioOpenAL		= module.plugin('audioOpenAL',  [discworld, openal], platforms=['pc'])
	package			= module.plugin('package',      [discworld])
	lua				= module.plugin('lua',          [discworld, lualib])
	squirrel		= module.plugin('squirrel',     [discworld, squirellib])
	angelcode		= module.plugin('angelcode',    [discworld, angelcodelib])

	discworld.plugins=[renderOpenGL, renderDx9, renderNull, physicsBullet, package, lua, squirrel, angelcode]
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

