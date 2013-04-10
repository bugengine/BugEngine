VERSION = "0.2.0"
APPNAME = "BugEngine"

top = '.'
out = '.build/waf'

def options(opt):
	opt.recurse('mak')

def configure(conf):
	conf.recurse('mak')

def build(bld):
	bld.recurse('mak')

	bld.external('3rdparty.zlib')
	bld.external('3rdparty.mak')

	bld.library('engine.kernel',				[])
	bld.library('engine.minitl',				bld.platforms+['engine.kernel'])
	bld.library('engine.core',					['3rdparty.mak', 'engine.minitl', 'engine.kernel'])
	bld.library('engine.network',				['engine.core'])
	bld.library('engine.rtti',					['engine.core', 'engine.network', '3rdparty.zlib'])
	bld.library('engine.filesystem',			['engine.core', 'engine.rtti'])
	bld.library('engine.resource',				['engine.core', 'engine.rtti', 'engine.filesystem'])
	bld.library('engine.scheduler',				['engine.core', 'engine.rtti', 'engine.resource'])
	bld.library('engine.world',					['engine.core', 'engine.rtti', 'engine.resource', 'engine.scheduler'])
	bld.library('engine.plugin',				['engine.core', 'engine.rtti', 'engine.filesystem', 'engine.resource', 'engine.scheduler'])

	bld.engine('engine.bugengine',				['engine.core', 'engine.rtti', 'engine.scheduler', 'engine.filesystem', 'engine.world', 'engine.plugin'])

	bld.external('3rdparty.DirectX9')
	bld.external('3rdparty.DirectX10')
	bld.external('3rdparty.DirectX11')
	bld.external('3rdparty.OpenGL')
	bld.external('3rdparty.OpenGLES2')
	bld.external('3rdparty.OpenCL')

	bld.external('3rdparty.OpenAL')
	bld.external('3rdparty.oggvorbis')

	bld.external('3rdparty.bullet')

	bld.external('3rdparty.lua')
	bld.external('3rdparty.squirrel')

	bld.plugin('plugin.debug.runtime',			['engine.bugengine'])
	bld.plugin('plugin.debug.assert',			['engine.bugengine', 'plugin.debug.runtime'])

	bld.plugin('plugin.scripting.package',		['engine.bugengine'])

	bld.plugin('plugin.physics.bullet',			['engine.bugengine', '3rdparty.bullet'])

	bld.plugin('plugin.graphics.3d',			['engine.bugengine'])
	bld.plugin('plugin.graphics.shadermodel1',	['engine.bugengine', 'plugin.graphics.3d'])
	bld.plugin('plugin.graphics.shadermodel2',	['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1'])
	bld.plugin('plugin.graphics.shadermodel3',	['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2'])
	bld.plugin('plugin.graphics.shadermodel4',	['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2', 'plugin.graphics.shadermodel3'])

	#bld.plugin('plugin.audio.AL',				['engine.bugengine', '3rdparty.OpenAL'])

	bld.plugin('plugin.scripting.lua',			['engine.bugengine', '3rdparty.lua'])
	bld.plugin('plugin.scripting.squirrel',		['engine.bugengine', '3rdparty.squirrel'])
	bld.plugin('plugin.input.input',			['engine.bugengine'])

	bld.plugin('plugin.kernel.cpu'	,			['engine.bugengine'])
	#bld.plugin('plugin.kernel.directcompute',	['engine.bugengine'])
	bld.plugin('plugin.kernel.opencl',			['engine.bugengine', '3rdparty.OpenCL'], features=['OpenCL'])

	bld.plugin('plugin.graphics.nullrender',	['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2', 'plugin.graphics.shadermodel3', 'plugin.graphics.shadermodel4'])
	bld.library('plugin.graphics.windowing',	['engine.bugengine', 'plugin.graphics.3d'], features=['GUI'])
	bld.plugin('plugin.graphics.GL4',			['engine.bugengine', 'plugin.graphics.windowing', '3rdparty.OpenGL'], features=['OpenGL', 'GUI'])
	bld.plugin('plugin.kernel.opencl_gl',		['engine.bugengine', '3rdparty.OpenGL', 'engine.plugin.kernel.opencl'], features=['OpenGL', 'OpenCL'])
	bld.plugin('plugin.graphics.Dx9',			['engine.bugengine', 'plugin.graphics.windowing', '3rdparty.DirectX9'], features=['DirectX9'])
	#bld.plugin('plugin.graphics.Dx10',			['engine.bugengine', 'plugin.graphics.windowing', '3rdparty.DirectX10'], features=['DirectX10'])
	#bld.plugin('plugin.graphics.Dx11',			['engine.bugengine', 'plugin.graphics.windowing', '3rdparty.DirectX11'], features=['DirectX11'])
	bld.plugin('plugin.graphics.GLES2',			['engine.bugengine', 'plugin.graphics.windowing', '3rdparty.OpenGLES2'], features=['OpenGLES2'])

	bld.external('3rdparty.scintilla')
	bld.plugin('game.bugeditor.ui',				['engine.bugengine', '3rdparty.scintilla'], platforms=['pc'])
	bld.game('game.bugeditor.main',				['engine.bugengine', 'game.bugeditor.ui', 'plugin.scripting.package'], platforms=['pc'])

	bld.game('game.samples.kernel',				['engine.bugengine', 'plugin.scripting.package'])

	bld.recurse('mak', name='plugins', once=False)

