"""
    Use this file to declare all libraries and modules in the bugengine
"""
from waflib import Options


def build_externals(bld):
    """
        Declares all external modules
    """
    bld.external('3rdparty.system.zlib')
    bld.external('3rdparty.system.minizip')
    bld.external('3rdparty.mak')
    bld.external('3rdparty.system.console')
    bld.external('3rdparty.system.X11')
    bld.external('3rdparty.system.win32')
    bld.external('3rdparty.graphics.DirectX9')
    bld.external('3rdparty.graphics.DirectX10')
    bld.external('3rdparty.graphics.DirectX11')
    bld.external('3rdparty.graphics.OpenGL')
    bld.external('3rdparty.graphics.OpenGLES2')
    bld.external('3rdparty.compute.OpenCL')
    bld.external('3rdparty.audio.OpenAL')
    bld.external('3rdparty.audio.oggvorbis')
    bld.external('3rdparty.physics.bullet')
    bld.external('3rdparty.system.freetype')
    bld.external('3rdparty.system.fontconfig')
    bld.external('3rdparty.scripting.lua')
    bld.external('3rdparty.scripting.squirrel')
    bld.external('3rdparty.system.ncurses')
    bld.external('3rdparty.scripting.tcltk')
    bld.external('3rdparty.scripting.python')


def build_bugengine(bld):
    """
        Declares the main library and entry point
    """
    bld.headers('engine.kernel',
                [],
                extra_public_includes=[bld.path.make_node('engine/kernel/api/generic')])
    bld.headers('engine.kernel.cpu',
                [],
                path='engine.kernel',
                extra_public_includes=[bld.path.make_node('engine/kernel/api/cpu')])
    bld.library('engine.minitl',
                bld.platforms+['engine.kernel'])
    bld.library('engine.core',
                ['3rdparty.mak', 'engine.minitl', 'engine.kernel'])
    bld.library('engine.network',
                ['engine.core'])
    bld.library('engine.rtti',
                ['engine.core', 'engine.network'],
                ['3rdparty.system.zlib'])
    bld.library('engine.filesystem',
                ['engine.core', 'engine.rtti'],
                ['3rdparty.system.minizip'])
    bld.library('engine.rttiparse',
                ['engine.core', 'engine.rtti', 'engine.filesystem'])
    bld.library('engine.settings',
                ['engine.rtti', 'engine.rttiparse'])
    bld.library('engine.resource',
                ['engine.core', 'engine.rtti', 'engine.filesystem'])
    bld.library('engine.scheduler',
                ['engine.core', 'engine.rtti', 'engine.resource'])
    bld.library('engine.world',
                ['engine.core', 'engine.rtti', 'engine.resource',
                 'engine.scheduler'])
    bld.library('engine.plugin',
                ['engine.core', 'engine.rtti', 'engine.filesystem',
                 'engine.resource', 'engine.scheduler'])
    bld.shared_library('engine.bugengine',
                       ['engine.core', 'engine.rtti', 'engine.rttiparse',
                        'engine.settings', 'engine.scheduler', 'engine.filesystem',
                        'engine.world', 'engine.plugin'])

    bld.engine('bugengine', ['engine.bugengine'], path='engine.main')


def build_plugins(bld):
    """
        Declares all plugins
    """
    bld.plugin('plugin.debug.runtime',
               ['engine.bugengine'])
    bld.plugin('plugin.debug.assert',
               ['engine.bugengine', 'plugin.debug.runtime'])

    bld.plugin('plugin.scripting.package',
               ['engine.bugengine'])

    bld.plugin('plugin.physics.bullet',
               ['engine.bugengine'],
               ['3rdparty.physics.bullet'])

    bld.plugin('plugin.graphics.3d',
               ['engine.bugengine'])
    bld.plugin('plugin.graphics.shadermodel1',
               ['engine.bugengine', 'plugin.graphics.3d'])
    bld.plugin('plugin.graphics.shadermodel2',
               ['engine.bugengine', 'plugin.graphics.3d',
                'plugin.graphics.shadermodel1'])
    bld.plugin('plugin.graphics.shadermodel3',
               ['engine.bugengine', 'plugin.graphics.3d',
                'plugin.graphics.shadermodel1',
                'plugin.graphics.shadermodel2'])
    bld.plugin('plugin.graphics.shadermodel4',
               ['engine.bugengine', 'plugin.graphics.3d',
                'plugin.graphics.shadermodel1',
                'plugin.graphics.shadermodel2',
                'plugin.graphics.shadermodel3'])

    #bld.plugin('plugin.audio.AL',
    #           ['engine.bugengine'],
    #           ['3rdparty.audio.OpenAL'])

    bld.plugin('plugin.scripting.lua',
               ['engine.bugengine'],
               ['3rdparty.scripting.lua'])
    bld.plugin('plugin.scripting.squirrel',
               ['engine.bugengine'],
               ['3rdparty.scripting.squirrel'])
    bld.plugin('plugin.input.input',
               ['engine.bugengine'])
    bld.shared_library('plugin.scripting.pythonlib',
                       ['engine.bugengine'],
                       platforms=['pc'])
    bld.plugin('plugin.scripting.python',
               ['engine.bugengine', 'plugin.scripting.pythonlib'],
               platforms=['pc'])
    bld.python_module('py_bugengine', ['engine.bugengine',
                                       'plugin.scripting.pythonlib'],
                      path='plugin.scripting.pythonmodule',
                      platforms=['pc'])
    if bld.env.PROJECTS:
        python_deps = ['3rdparty.scripting.python%s'%version.replace('.', '')
                       for version in bld.env.PYTHON_VERSIONS]
        bld.plugin('plugin.scripting.pythonbinding',
                   ['engine.bugengine', 'plugin.scripting.pythonlib'] + python_deps)
    else:
        for version in bld.env.PYTHON_VERSIONS:
            short_version = version.replace('.', '')
            bld.plugin('plugin.scripting.python%s' % short_version,
                       ['engine.bugengine', 'plugin.scripting.python'],
                       ['3rdparty.scripting.python%s'%short_version],
                       path='plugin.scripting.pythonbinding',
                       features=['python%s'%version])

    bld.plugin('plugin.compute.cpu',
               ['engine.bugengine'],
               extra_tasks=['generate_cpu_variants'])
    #bld.plugin('plugin.compute.glcompute',
    #		   ['engine.bugengine', 'plugin.compute.cpu'])
    #bld.plugin('plugin.compute.directcompute',
    #		   ['engine.bugengine', 'plugin.compute.cpu'])
    #bld.plugin('plugin.compute.cuda',
    #		   ['engine.bugengine', 'plugin.compute.cpu'])
    bld.plugin('plugin.compute.opencl',
               ['engine.bugengine', 'plugin.compute.cpu'],
               ['3rdparty.compute.OpenCL'],
               features=['OpenCL'])
    bld.plugin('plugin.compute.opencl_gl',
               ['engine.bugengine', 'plugin.graphics.GL4',
                'plugin.compute.opencl', 'plugin.compute.cpu'],
               ['3rdparty.graphics.OpenGL', '3rdparty.compute.OpenCL'],
               features=['OpenGL', 'OpenCL', 'GUI'])

    bld.plugin('plugin.graphics.nullrender',
               ['engine.bugengine', 'plugin.graphics.3d',
                'plugin.graphics.shadermodel1',
                'plugin.graphics.shadermodel2',
                'plugin.graphics.shadermodel3',
                'plugin.graphics.shadermodel4'])
    bld.plugin('plugin.graphics.windowing',
               ['engine.bugengine', 'plugin.graphics.3d'],
               ['3rdparty.system.X11', '3rdparty.graphics.OpenGL'],
               features=['GUI'])
    bld.plugin('plugin.graphics.GL4',
               ['engine.bugengine', 'plugin.graphics.windowing'],
               ['3rdparty.graphics.OpenGL'],
               features=['OpenGL', 'GUI'])
    bld.plugin('plugin.graphics.Dx9',
               ['engine.bugengine', 'plugin.graphics.windowing'],
               ['3rdparty.graphics.DirectX9'],
               features=['DirectX9', 'GUI'])
    #bld.plugin('plugin.graphics.Dx10',
    #           ['engine.bugengine', 'plugin.graphics.windowing'],
    #           ['3rdparty.graphics.DirectX10'],
    #           features=['DirectX10', 'GUI'])
    #bld.plugin('plugin.graphics.Dx11',
    #           ['engine.bugengine', 'plugin.graphics.windowing'],
    #           ['3rdparty.graphics.DirectX11'],
    #           features=['DirectX11', 'GUI'])
    bld.plugin('plugin.graphics.GLES2',
               ['engine.bugengine', 'plugin.graphics.windowing'],
               ['3rdparty.graphics.OpenGLES2'],
               features=['OpenGLES2', 'GUI'])

    bld.plugin('plugin.graphics.text',
               ['engine.bugengine', 'plugin.graphics.3d'],
               ['3rdparty.system.freetype', '3rdparty.system.fontconfig'])

    bld.plugin('plugin.ui.console',
               ['engine.bugengine'],
               ['3rdparty.system.ncurses'],
               platforms=['pc'])

    bld.plugin('tool.bugeditor.ui',
               ['engine.bugengine'],
               platforms=['pc'])


def build_games(bld):
    """
        Declares all games/samples/tools/autotests
    """
    bld.game('bugeditor',
             ['engine.bugengine', 'tool.bugeditor.ui',
              'plugin.scripting.package'],
             path='tool.bugeditor.main',
             platforms=['pc'])
    bld.game('sample.particlerain',
             ['engine.bugengine', 'plugin.scripting.package'])
    bld.game('sample.text',
             ['engine.bugengine', 'plugin.scripting.package', 'plugin.graphics.3d'])
    bld.game('sample.python',
             ['engine.bugengine', 'plugin.scripting.package'])
    bld.game('sample.lua',
             ['engine.bugengine', 'plugin.scripting.package', 'plugin.scripting.lua'])
    bld.game('help', ['engine.bugengine', 'plugin.ui.console',
                      'plugin.scripting.package'],
             path='tool.help', platforms=['pc'])
    if Options.options.tests:
        bld.game('test.clt', ['plugin.compute.cpu'])
        bld.game('test.settings', ['engine.bugengine'])


def build(bld):
    """
        Declares each bugengine module and their dependencies
    """
    build_externals(bld)
    build_bugengine(bld)
    build_plugins(bld)
    build_games(bld)
