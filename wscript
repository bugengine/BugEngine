VERSION = "0.1.0"
APPNAME = "BugEngine"

from mak import module
import os
import Environment
import Scripting
import Logs

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

	core			= module.library('core',        [dbghelp, win32, pthreads])
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
	editor			= module.tool('editor',         [core, rtti, system, input, graphics, audio, physics, mobile, main], platforms=['pc'])

	win32			= module.library('win32',       [discworld], category='plugin', platforms=['win32'])
	X				= module.library('X',           [discworld, X11], category='plugin', platforms=['posix'])
	physicsBullet	= module.plugin('physicsBullet',[discworld, bullet])
	renderOpenGL	= module.plugin('renderOpenGL', [discworld, win32, X, opengl], platforms=['pc'])
	renderDx9		= module.plugin('renderDx9',    [discworld, win32, cgDx, directx9], platforms=['win32'])
	#audioOpenAL		= module.plugin('audioOpenAL',  [discworld, openal], platforms=['pc'])
	lua				= module.plugin('lua',          [discworld, lualib])
	squirrel		= module.plugin('squirrel',     [discworld, squirellib])

	discworld.plugins=[renderOpenGL, renderDx9, physicsBullet]
	discworld.post(bld)
	editor.post(bld)
	#testsuite		= module.library('testsuite', category='test')
	#atomic_test		= module.test('atomic', [core, testsuite]).post(bld)

