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
	if not bld.variant:
		if not bld.env.PROJECTS:
			Options.commands[:0] = [bld.__class__.cmd+'_' + i for i in bld.env.BUILD_VARIANTS]
			return

	dbghelp			= module.external('dbghelp')
	win32			= module.external('win32')

	mak				= module.external('mak')

	core			= module.library('core',        [dbghelp, win32, mak])
	network			= module.library('network',     [core])
	rtti			= module.library('rtti',        [core, network] )
	system			= module.library('system',      [core, rtti] )
	world			= module.library('world',       [core, rtti, system] )
	main			= module.library('main',        [core, rtti, system, world])

	bld.game		= module.game('discworld',      [core, rtti, system, world, main])

	bld.recurse('.', name='plugins', once=False)
	bld.game.post(bld)


def plugins(bld):
	bld.game.plugins = []

	directx9		= module.external('DirectX9')
	directx10		= module.external('DirectX10')
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


	editor			= module.plugin('editor',   	    [bld.game], platforms=['pc'])

	bullet			= module.plugin('bullet',			[bld.game, bulletengine])

	_3d				= module.plugin('3d', 				[bld.game])
	shadermodel1	= module.plugin('shadermodel1', 	[bld.game, _3d])
	shadermodel2	= module.plugin('shadermodel2', 	[bld.game, _3d, shadermodel1])
	shadermodel3	= module.plugin('shadermodel3', 	[bld.game, _3d, shadermodel1, shadermodel2])
	shadermodel4	= module.plugin('shadermodel4', 	[bld.game, _3d, shadermodel1, shadermodel2, shadermodel3])

	windowing		= module.library('windowing',   	[bld.game, _3d, X11, win32], category='plugin')
	gl				= module.plugin('GL4', 				[bld.game, windowing, opengl, _3d], platforms=['pc'])
	Dx9				= module.plugin('Dx9',          	[bld.game, windowing, cgDx, directx9, _3d], platforms=['win32'])
	#Dx10			= module.plugin('Dx10',         	[bld.game, windowing, cgDx, directx10, _3d], platforms=['win32'])
	nullrender		= module.plugin('nullrender', 		[bld.game, _3d, shadermodel4])

	#AL				= module.plugin('AL',  				[bld.game, openal], platforms=['pc'])

	package			= module.plugin('package',      	[bld.game])
	packagebuilder	= module.plugin('packagebuilder',	[bld.game], category='plugin')
	lua				= module.plugin('lua',          	[bld.game, lualib])
	squirrel		= module.plugin('squirrel',     	[bld.game, squirellib])
	input			= module.plugin('input', 			[bld.game])

	bld.game.plugins+=[
		_3d, shadermodel1, shadermodel2, shadermodel3, shadermodel4, nullrender,
		bullet,
		editor,
		package, packagebuilder, lua, squirrel,
		input]


	if win32 or X11 or cocoa:
		windowing	= module.library('windowing',   [bld.game, _3d, X11, win32], category='plugin')
		if opengl:
			gl		= module.plugin('GL4', 			[bld.game, windowing, opengl, _3d], platforms=['pc'])
			bld.game.plugins.append(gl)
		if directx9:
			Dx9		= module.plugin('DX9',          [bld.game, windowing, cgDx, directx9, _3d], platforms=['win32'])
			bld.game.plugins.append(Dx9)
		#if diretx10:
			#Dx10	= module.plugin('DX10',         [bld.game, windowing, cgDx, directx10, _3d], platforms=['win32'])
			#bld.game.plugins.append(Dx10)

	bld.recurse('mak', name='plugins', once=False)
