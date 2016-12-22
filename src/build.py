def build(bld):
    bld.external('3rdparty.zlib')
    bld.external('3rdparty.minizip')
    bld.external('3rdparty.mak')

    bld.library('engine.kernel',
                [])
    bld.library('engine.minitl',
                bld.platforms+['engine.kernel'])
    bld.library('engine.core',
                ['3rdparty.mak', 'engine.minitl', 'engine.kernel'])
    bld.library('engine.network',
                ['engine.core'])
    bld.library('engine.rtti',
                ['engine.core', 'engine.network', '3rdparty.zlib'])
    bld.library('engine.filesystem',
                ['engine.core', 'engine.rtti', '3rdparty.minizip'])
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
                       ['engine.core', 'engine.rtti', 'engine.scheduler',
                        'engine.filesystem', 'engine.world', 'engine.plugin'])

    bld.engine('bugengine', ['engine.bugengine'], path='engine.main')

    bld.external('3rdparty.console')
    bld.external('3rdparty.X11')
    bld.external('3rdparty.win32')
    bld.external('3rdparty.DirectX9')
    bld.external('3rdparty.DirectX10')
    bld.external('3rdparty.DirectX11')
    bld.external('3rdparty.OpenGL')
    bld.external('3rdparty.OpenGLES2')
    bld.external('3rdparty.OpenCL')

    bld.external('3rdparty.OpenAL')
    bld.external('3rdparty.oggvorbis')

    bld.external('3rdparty.bullet')
    bld.external('3rdparty.freetype')
    bld.external('3rdparty.fontconfig')

    bld.external('3rdparty.lua')
    bld.external('3rdparty.squirrel')
    bld.external('3rdparty.ncurses')
    bld.external('3rdparty.tcltk')
    bld.external('3rdparty.python')

    bld.plugin('plugin.debug.runtime',
               ['engine.bugengine'])
    bld.plugin('plugin.debug.assert',
               ['engine.bugengine', 'plugin.debug.runtime'])

    bld.plugin('plugin.scripting.package',
               ['engine.bugengine'])

    bld.plugin('plugin.physics.bullet',
               ['engine.bugengine', '3rdparty.bullet'])

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
                'plugin.graphics.shadermodel3'],
             platforms=['pc'])

    #bld.plugin('plugin.audio.AL',
    #		   ['engine.bugengine', '3rdparty.OpenAL'])

    bld.plugin('plugin.scripting.lua',
               ['engine.bugengine', '3rdparty.lua'])
    bld.plugin('plugin.scripting.squirrel',
               ['engine.bugengine', '3rdparty.squirrel'])
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
        python_deps = ['3rdparty.python%s'%version.replace('.', '')
                            for version in bld.env.PYTHON_VERSIONS]
        bld.plugin('plugin.scripting.pythonbinding',
                   ['engine.bugengine', 'plugin.scripting.pythonlib'] + python_deps)
    else:
        for version in bld.env.PYTHON_VERSIONS:
            bld.plugin('plugin.scripting.python%s' % version.replace('.', ''),
                       ['engine.bugengine', 'plugin.scripting.python',
                        '3rdparty.python%s'%version.replace('.', '')],
                       path='plugin.scripting.pythonbinding',
                       features=['python%s'%version])

    bld.plugin('plugin.kernel.cpu',
               ['engine.bugengine'])
    #bld.plugin('plugin.kernel.directcompute',
    #		   ['engine.bugengine'])
    bld.plugin('plugin.kernel.opencl',
               ['engine.bugengine', '3rdparty.OpenCL', '3rdparty.OpenGL'],
               features=['OpenCL'])

    bld.plugin('plugin.graphics.nullrender',
               ['engine.bugengine', 'plugin.graphics.3d',
                'plugin.graphics.shadermodel1',
                'plugin.graphics.shadermodel2',
                'plugin.graphics.shadermodel3',
                'plugin.graphics.shadermodel4'])
    bld.plugin('plugin.graphics.windowing',
               ['engine.bugengine', 'plugin.graphics.3d',
                '3rdparty.X11', '3rdparty.OpenGL'],
               features=['GUI'])
    bld.plugin('plugin.graphics.GL4',
               ['engine.bugengine', 'plugin.graphics.windowing',
                '3rdparty.OpenGL'],
               features=['OpenGL', 'GUI'])
    bld.plugin('plugin.kernel.opencl_gl',
               ['engine.bugengine', 'plugin.graphics.GL4',
                '3rdparty.OpenGL', '3rdparty.OpenCL',
                'plugin.kernel.opencl'],
               features=['OpenGL', 'OpenCL', 'GUI'])
    bld.plugin('plugin.graphics.Dx9',
               ['engine.bugengine', 'plugin.graphics.windowing',
                '3rdparty.DirectX9'],
               features=['DirectX9', 'GUI'])
    #bld.plugin('plugin.graphics.Dx10',
    #           ['engine.bugengine', 'plugin.graphics.windowing',
    #            '3rdparty.DirectX10'],
    #           features=['DirectX10', 'GUI'])
    #bld.plugin('plugin.graphics.Dx11',
    #           ['engine.bugengine', 'plugin.graphics.windowing',
    #            '3rdparty.DirectX11'],
    #           features=['DirectX11', 'GUI'])
    bld.plugin('plugin.graphics.GLES2',
               ['engine.bugengine', 'plugin.graphics.windowing',
                '3rdparty.OpenGLES2'],
               features=['OpenGLES2', 'GUI'])

    bld.plugin('plugin.graphics.text',
               ['engine.bugengine', 'plugin.graphics.3d',
                '3rdparty.freetype', '3rdparty.fontconfig'])

    bld.plugin('plugin.ui.console',
               ['engine.bugengine', '3rdparty.ncurses'],
               platforms=['pc'])

    bld.external('3rdparty.scintilla')
    bld.plugin('tool.bugeditor.ui',
               ['engine.bugengine', '3rdparty.scintilla'],
               platforms=['pc'])
    bld.game('tool.bugeditor.main',
             ['engine.bugengine', 'game.bugeditor.ui',
              'plugin.scripting.package'],
             platforms=['pc'])

    bld.game('sample.kernel',
             ['engine.bugengine', 'plugin.scripting.package'])
    bld.game('sample.text',
             ['engine.bugengine', 'plugin.scripting.package', 'plugin.graphics.3d'])
    bld.game('sample.python',
             ['engine.bugengine', 'plugin.scripting.package'])
    bld.game('help', ['engine.bugengine', 'plugin.ui.console',
              'plugin.scripting.package'],
             path='tool.help', platforms=['pc'])

