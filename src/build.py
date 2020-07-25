"""
    Use this file to declare all libraries and modules in the bugengine
"""
from waflib import Options


def build_externals(bld):
    """
        Declares all external modules
    """
    bld.external('3rdparty.system.zlib')
    bld.external('3rdparty.system.console')
    bld.external('3rdparty.system.X11')
    bld.external('3rdparty.system.win32')
    bld.external('3rdparty.graphics.DirectX')
    bld.external('3rdparty.graphics.OpenGL')
    bld.external('3rdparty.graphics.OpenGLES2')
    bld.external('3rdparty.graphics.freetype')
    bld.external('3rdparty.compute.OpenCL')
    bld.external('3rdparty.compute.CUDA')
    bld.external('3rdparty.physics.bullet')
    bld.external('3rdparty.scripting.lua')
    bld.external('3rdparty.scripting.python')


def build_bugengine(bld):
    """
        Declares the main library and entry point
    """
    bld.headers('engine.mak', path=bld.bugenginenode.find_node('mak'), features=['Makefile'])
    bld.headers('engine.kernel', [], extra_public_includes=[bld.path.make_node('engine/kernel/api.cpu')])
    bld.library('engine.minitl', bld.platforms + ['engine.mak', 'engine.kernel'])
    bld.library('engine.core', ['engine.minitl', 'engine.kernel'])
    bld.library('engine.network', ['engine.core'])
    bld.library('engine.rtti', ['engine.core', 'engine.network'], ['3rdparty.system.zlib'])
    bld.library('engine.filesystem', ['engine.core', 'engine.rtti'], ['3rdparty.system.minizip'])
    bld.library('engine.rttiparse', ['engine.core', 'engine.rtti', 'engine.filesystem'])
    bld.library('engine.settings', ['engine.rtti', 'engine.rttiparse'])
    bld.library('engine.resource', ['engine.core', 'engine.rtti', 'engine.filesystem'])
    bld.library('engine.scheduler', ['engine.core', 'engine.rtti', 'engine.resource', 'engine.settings'])
    bld.library('engine.world', ['engine.core', 'engine.rtti', 'engine.resource', 'engine.scheduler'])
    bld.library(
        'engine.plugin', ['engine.core', 'engine.rtti', 'engine.filesystem', 'engine.resource', 'engine.scheduler']
    )
    bld.shared_library(
        'engine.bugengine', [
            'engine.core', 'engine.rtti', 'engine.rttiparse', 'engine.settings', 'engine.scheduler',
            'engine.filesystem', 'engine.world', 'engine.plugin'
        ]
    )

    bld.engine('bugengine', ['engine.bugengine'], path=bld.path.find_node('engine/main'))


def build_plugins(bld):
    """
        Declares all plugins
    """
    bld.plugin('plugin.debug.runtime', ['engine.bugengine'])
    bld.plugin('plugin.debug.assert', ['engine.bugengine', 'plugin.debug.runtime'])

    bld.plugin('plugin.scripting.package', ['engine.bugengine'])

    bld.plugin('plugin.physics.bullet', ['engine.bugengine'], ['3rdparty.physics.bullet'])

    bld.plugin('plugin.graphics.3d', ['engine.bugengine'])
    bld.plugin('plugin.graphics.shadermodel1', ['engine.bugengine', 'plugin.graphics.3d'])
    bld.plugin(
        'plugin.graphics.shadermodel2', ['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1']
    )
    bld.plugin(
        'plugin.graphics.shadermodel3',
        ['engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2']
    )
    bld.plugin(
        'plugin.graphics.shadermodel4', [
            'engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2',
            'plugin.graphics.shadermodel3'
        ]
    )

    #bld.plugin('plugin.audio.AL',
    #           ['engine.bugengine'],
    #           ['3rdparty.audio.OpenAL'])

    bld.plugin('plugin.scripting.lua', ['engine.bugengine'], ['3rdparty.scripting.lua'])
    bld.plugin('plugin.input.input', ['engine.bugengine'])
    bld.shared_library('plugin.scripting.pythonlib', ['engine.bugengine'], conditions=['python'])
    bld.plugin('plugin.scripting.python', ['engine.bugengine', 'plugin.scripting.pythonlib'], conditions=['python'])
    bld.python_module(
        'py_bugengine', ['engine.bugengine', 'plugin.scripting.pythonlib'],
        path=bld.path.find_node('plugin/scripting/pythonmodule'),
        conditions=['python']
    )
    if bld.env.PROJECTS:
        python_deps = ['3rdparty.scripting.python%s' % version.replace('.', '') for version in bld.env.PYTHON_VERSIONS]
        bld.plugin('plugin.scripting.pythonbinding', ['engine.bugengine', 'plugin.scripting.pythonlib'] + python_deps)
    else:
        for version in bld.env.PYTHON_VERSIONS:
            short_version = version.replace('.', '')
            bld.plugin(
                'plugin.scripting.python%s' % short_version, ['engine.bugengine', 'plugin.scripting.python'],
                ['3rdparty.scripting.python%s' % short_version],
                path=bld.path.find_node('plugin/scripting/pythonbinding'),
                conditions=['python%s' % version]
            )

    bld.plugin('plugin.compute.cpu', ['engine.bugengine'], features=['bugengine:cpu:variants'])
    bld.plugin(
        'plugin.compute.opencl', ['engine.bugengine', 'plugin.compute.cpu'], ['3rdparty.compute.OpenCL'],
        conditions=['OpenCL'],
        extra_defines=['CL_TARGET_OPENCL_VERSION=120'],
        extra_public_defines=['CL_TARGET_OPENCL_VERSION=120']
    )
    #bld.plugin(
    #    'plugin.compute.opencl_gl',
    #    ['engine.bugengine', 'plugin.graphics.GL4', 'plugin.compute.opencl', 'plugin.compute.cpu'],
    #    ['3rdparty.graphics.OpenGL', '3rdparty.compute.OpenCL'],
    #    features=['OpenGL', 'OpenCL', 'GUI']
    #)
    bld.plugin('plugin.compute.cuda', ['engine.bugengine'], ['3rdparty.compute.CUDA'], conditions=['cuda'])

    bld.plugin(
        'plugin.graphics.nullrender', [
            'engine.bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2',
            'plugin.graphics.shadermodel3', 'plugin.graphics.shadermodel4'
        ]
    )
    bld.plugin(
        'plugin.graphics.windowing', ['engine.bugengine', 'plugin.graphics.3d'],
        ['3rdparty.system.X11', '3rdparty.graphics.OpenGL'],
        conditions=['GUI']
    )
    bld.plugin(
        'plugin.graphics.GL4', ['engine.bugengine', 'plugin.graphics.windowing'], ['3rdparty.graphics.OpenGL'],
        conditions=['OpenGL', 'GUI']
    )
    bld.plugin(
        'plugin.graphics.Dx9', ['engine.bugengine', 'plugin.graphics.windowing'], ['3rdparty.graphics.DirectX9'],
        conditions=['DirectX9', 'GUI']
    )
    #bld.plugin('plugin.graphics.Dx10',
    #           ['engine.bugengine', 'plugin.graphics.windowing'],
    #           ['3rdparty.graphics.DirectX10'],
    #           conditions=['DirectX10', 'GUI'])
    #bld.plugin('plugin.graphics.Dx11',
    #           ['engine.bugengine', 'plugin.graphics.windowing'],
    #           ['3rdparty.graphics.DirectX11'],
    #           conditions=['DirectX11', 'GUI'])
    bld.plugin(
        'plugin.graphics.GLES2', ['engine.bugengine', 'plugin.graphics.windowing'], ['3rdparty.graphics.OpenGLES2'],
        conditions=['OpenGLES2', 'GUI']
    )

    bld.plugin(
        'plugin.graphics.text', ['engine.bugengine', 'plugin.graphics.3d'],
        ['3rdparty.graphics.freetype', '3rdparty.system.fontconfig']
    )

    bld.plugin('tool.bugeditor.ui', ['engine.bugengine'])


def build_games(bld):
    """
        Declares all games/samples/tools/autotests
    """
    bld.game(
        'bugeditor', ['engine.bugengine', 'tool.bugeditor.ui', 'plugin.scripting.package'],
        path=bld.path.find_node('tool/bugeditor/main')
    )
    bld.game('sample.text', ['engine.bugengine', 'plugin.scripting.package', 'plugin.graphics.3d'])
    bld.game('sample.python', ['engine.bugengine', 'plugin.scripting.package'])
    bld.game('sample.lua', ['engine.bugengine', 'plugin.scripting.package', 'plugin.scripting.lua'])
    bld.game('help', ['engine.bugengine', 'plugin.scripting.package'], path=bld.path.find_node('tool/help'))
    if Options.options.tests:
        bld.game('test.settings', ['engine.bugengine'])
        bld.game('test.compute.copy', ['engine.bugengine', 'plugin.scripting.package'])


def build(bld):
    """
        Declares each bugengine module and their dependencies
    """
    build_externals(bld)
    build_bugengine(bld)
    build_plugins(bld)
    build_games(bld)
