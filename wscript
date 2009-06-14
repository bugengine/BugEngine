VERSION = "0.1.0"
APPNAME = "BugEngine"

from mak import module
import os

srcdir = '.'
blddir = '.build'

def set_options(opt):
	opt.sub_options('mak')

def configure(conf):
	conf.sub_config('mak')

def build(bld):
	bld.add_subdirs('mak')

	dbghelp			= module.external('dbghelp')
	directx9		= module.external('DirectX9')
	directx10		= module.external('DirectX10')
	cgDx			= module.external('CgDx')
	cgGL			= module.external('CgGL')

	freetype		= module.external('freetype')

	openal			= module.external('OpenAL')
	oggvorbis		= module.external('oggvorbis')

	bullet			= module.external('bullet')

	win32			= module.external('win32')

	core			= module.library('core',[dbghelp, win32])
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

	w32				= module.library('w32', [discworld], category='plugin', platforms=['win32'])
	renderDx9		= module.plugin('renderDx9', [discworld, w32, cgDx, directx9], platforms=['win32'])
	#renderOpenGL	 = module.plugin('renderOpenGL', [discworld, w32, cgGL, openGL])

	#testsuite		= module.library('testsuite', category='test')
	#atomic_test		= module.test('atomic', [core, testsuite])
