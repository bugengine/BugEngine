VERSION = "0.1.0"
APPNAME = "BugEngine"

from mak import module
import os

srcdir = '.'
blddir = '.build/waf'

def set_options(opt):
	opt.sub_options('mak')

def configure(conf):
	conf.sub_config('mak')

def build(bld):
	bld.add_subdirs('mak')

	dbghelp			= module.external('dbghelp')
	directx9		= module.external('DirectX9')
	directx10		= module.external('DirectX10')
	opengl			= module.external('OpenGL')
	cgDx			= module.external('CgDx')
	cgGL			= module.external('CgGL')
	X11				= module.external('X11')

	freetype		= module.external('freetype')

	openal			= module.external('OpenAL')
	oggvorbis		= module.external('oggvorbis')

	bullet			= module.external('bullet')

	win32			= module.external('win32')
	libwii			= module.external('libwii')
	pthreads		= module.external('pthreads')

	lualib			= module.external('lualib')
	squirellib		= module.external('squirrellib')

	core			= module.library('core',[dbghelp, win32, libwii, pthreads])
	network			= module.library('network',[core])
	rtti			= module.library('rtti',[core,network] )
	system			= module.library('system',[core,rtti] )
	data			= module.library('data',[core,rtti,system] )
	input			= module.library('input',[core,rtti] )
	graphics		= module.library('graphics',[core, rtti, system, input, freetype ] )
	sound			= module.library('sound',[core,rtti,system, openal, oggvorbis] )
	physics			= module.library('physics',[core,rtti,system, bullet] )
	mobile			= module.library('mobile',[core,rtti,system,graphics, sound, physics, input] )
	main			= module.library('main', [core, rtti, system, data, input, graphics, sound, physics, mobile])

	discworld		= module.game('discworld', [core, rtti, system, data, input, graphics, sound, physics, mobile, main])
	editor			= module.tool('editor', [core, rtti, system, data, input, graphics, sound, physics, mobile, main], platforms=['pc'])

	win32			= module.library('win32', [discworld], category='plugin', platforms=['win32'])
	X				= module.library('X', [discworld, X11], category='plugin', platforms=['posix'])
	renderOpenGL	= module.plugin('renderOpenGL', [discworld, win32, X, opengl], platforms=['pc'])
	renderDx9		= module.plugin('renderDx9', [discworld, win32, cgDx, directx9], platforms=['win32'])
	lua				= module.plugin('lua', [discworld, lualib])
	squirrel		= module.plugin('squirrel', [discworld, squirellib])

	discworld.plugins=[renderOpenGL, renderDx9, lua, squirrel]
	discworld.post(bld)
	editor.post(bld)
	#testsuite		= module.library('testsuite', category='test')
	#atomic_test		= module.test('atomic', [core, testsuite]).post(bld)
