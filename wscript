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


	editor			= module.plugin('editor',   	    [], platforms=['pc'])

	bullet			= module.plugin('bullet',			[bulletengine])

	_3d				= module.plugin('3d', 				[])
	shadermodel1	= module.plugin('shadermodel1', 	[_3d])
	shadermodel2	= module.plugin('shadermodel2', 	[_3d, shadermodel1])
	shadermodel3	= module.plugin('shadermodel3', 	[_3d, shadermodel1, shadermodel2])
	shadermodel4	= module.plugin('shadermodel4', 	[_3d, shadermodel1, shadermodel2, shadermodel3])


	#AL				= module.plugin('AL',  				[openal], platforms=['pc'])

	package			= module.plugin('package',      	[])
	packagebuilder	= module.plugin('packagebuilder',	[], category='plugin')
	lua				= module.plugin('lua',          	[lualib])
	squirrel		= module.plugin('squirrel',     	[squirellib])
	input			= module.plugin('input', 			[])

	nullrender		= module.plugin('nullrender', 		[_3d, shadermodel4])
	if win32 or X11 or cocoa:
		windowing	= module.library('windowing',   [bld.game, _3d, X11, win32], category='plugin')
		if opengl:
			gl		= module.plugin('GL4', 			[bld.game, windowing, opengl, _3d], platforms=['pc'])
		if directx9:
			Dx9		= module.plugin('DX9',          [bld.game, windowing, cgDx, directx9, _3d], platforms=['win32'])
		#if diretx10:
			#Dx10	= module.plugin('DX10',         [bld.game, windowing, cgDx, directx10, _3d], platforms=['win32'])

	bld.recurse('mak', name='plugins', once=False)

