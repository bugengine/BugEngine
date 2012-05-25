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
	bld.recurse('mak')
	if not bld.variant and not bld.env.PROJECTS:
		Options.commands[:0] = [bld.__class__.cmd+'_' + i for i in bld.env.BUILD_VARIANTS]
		return

	dbghelp			= module.external('dbghelp')
	win32			= module.external('win32')
	zlib			= module.external('zlib')

	mak				= module.external('mak')

	core			= module.library('core',		[dbghelp, win32, mak])
	network			= module.library('network',		[core])
	rtti			= module.library('rtti',		[core, network, zlib] )
	system			= module.library('system',		[core, rtti] )
	world			= module.library('world',		[core, rtti, system] )

	bld.game		= module.engine('bugengine',	[core, rtti, system, world])
	bld.recurse('.', name='plugins', once=False)
	bld.game.post(bld)


def plugins(bld):
	directx9		= module.external('DirectX9')
	directx10		= module.external('DirectX10')
	directx11		= module.external('DirectX11')
	opengl			= module.external('OpenGL')
	cgDx			= module.external('CgDx')
	cgGL			= module.external('CgGL')
	X11				= module.external('X11')
	cocoa			= module.external('cocoa')
	win32			= module.external('win32')

	freetype		= module.external('freetype')

	openal			= module.external('OpenAL')
	oggvorbis		= module.external('oggvorbis')

	bulletengine	= module.external('bulletengine')

	lualib			= module.external('lualib')
	squirellib		= module.external('squirrellib')


	package			= module.plugin('scripting.package',		[])

	bullet			= module.plugin('physics.bullet',			[bulletengine])

	_3d				= module.plugin('graphics.3d',				[freetype])
	shadermodel1	= module.plugin('graphics.shadermodel1',	[_3d])
	shadermodel2	= module.plugin('graphics.shadermodel2',	[_3d, shadermodel1])
	shadermodel3	= module.plugin('graphics.shadermodel3',	[_3d, shadermodel1, shadermodel2])
	shadermodel4	= module.plugin('graphics.shadermodel4',	[_3d, shadermodel1, shadermodel2, shadermodel3])

	#AL				= module.plugin('audio.AL',					[openal])

	lua				= module.plugin('scripting.lua',			[lualib])
	squirrel		= module.plugin('scripting.squirrel',		[squirellib])
	input			= module.plugin('input.input',				[])

	nullrender		= module.plugin('graphics.nullrender',		[_3d, shadermodel4])
	if win32 or X11 or cocoa:
		windowing	= module.library('graphics.windowing',		[bld.game, _3d, X11, win32], category='plugin')
		if opengl:
			gl		= module.plugin('graphics.GL4',				[bld.game, windowing, opengl, _3d])
		if directx9:
			Dx9		= module.plugin('graphics.DX9',				[bld.game, windowing, cgDx, directx9, _3d])
		#if diretx10:
			#Dx10	= module.plugin('graphics.DX10',			[bld.game, windowing, cgDx, directx10, _3d])
		#if diretx11:
			#Dx11	= module.plugin('graphics.DX11',			[bld.game, windowing, cgDx, directx11, _3d])


	scintilla		= module.external('scintilla')
	bugeditor = lambda: None
	bugeditor.ui	= module.plugin('bugeditor.ui',				[scintilla], platforms=['pc'], category='game')
	bugeditor		= module.game('bugeditor',					[bugeditor.ui, package], platforms=['pc'])

	bld.recurse('mak', name='plugins', once=False)

