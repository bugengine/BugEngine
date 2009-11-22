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
	pthreads		= module.external('pthreads')

	lualib			= module.external('lualib')
	squirellib		= module.external('squirrellib')

	shared = True
	if shared:
		lib = module.shared_library
	else:
		lib = module.library

	core			= lib('core',[dbghelp, win32, pthreads])
	network			= lib('network',[core])
	rtti			= lib('rtti',[core,network] )
	system			= lib('system',[core,rtti] )
	data			= lib('data',[core,rtti,system] )
	input			= lib('input',[core,rtti] )
	graphics		= lib('graphics',[core, rtti, system, input, freetype ] )
	sound			= lib('sound',[core,rtti,system, openal, oggvorbis] )
	physics			= lib('physics',[core,rtti,system, bullet] )
	mobile			= lib('mobile',[core,rtti,system,graphics, sound, physics, input] )
	main			= module.library('main', [core, rtti, system, data, input, graphics, sound, physics, mobile])
	discworld		= module.game('discworld', [core, rtti, system, data, input, graphics, sound, physics, mobile, main]).post(bld)

	if shared:
		depends = [core, rtti, system, data, input, graphics, sound, physics, mobile, discworld]
	else:
		depends = [discworld]
	win32			= module.library('win32', depends, category='plugin', platforms=['win32'])
	X				= module.library('X', depends+[X11], category='plugin', platforms=['posix'])
	renderOpenGL	= module.plugin('renderOpenGL', depends+[win32, X, opengl]).post(bld)
	renderDx9		= module.plugin('renderDx9', depends+[win32, cgDx, directx9], platforms=['win32']).post(bld)
	lua				= module.plugin('lua', depends+[lualib]).post(bld)
	squirrel		= module.plugin('squirrel', depends+[squirellib]).post(bld)

	editor			= module.tool('editor', [core, rtti, system, data, input, graphics, sound, physics, mobile, main], platforms=['win32', 'posix']).post(bld)

	#testsuite		= module.library('testsuite', category='test')
	#atomic_test		= module.test('atomic', [core, testsuite]).post(bld)
