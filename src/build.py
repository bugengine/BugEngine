"""
    Use this file to declare all libraries and modules in the bugengine
"""
from waflib import Options


def build_externals(bld):
    """
        Declares all external modules
    """
    bld.external('bugengine.3rdparty.system.zlib')
    bld.external('bugengine.3rdparty.system.console')
    bld.external('bugengine.3rdparty.system.X11')
    bld.external('bugengine.3rdparty.system.win32')
    bld.external('bugengine.3rdparty.graphics.DirectX')
    bld.external('bugengine.3rdparty.graphics.OpenGL')
    bld.external('bugengine.3rdparty.graphics.OpenGLES2')
    bld.external('bugengine.3rdparty.graphics.freetype')
    bld.external('bugengine.3rdparty.gui.gtk3')
    bld.external('bugengine.3rdparty.compute.OpenCL')
    bld.external('bugengine.3rdparty.compute.CUDA')
    bld.external('bugengine.3rdparty.physics.bullet')
    bld.external('bugengine.3rdparty.scripting.lua')
    bld.external('bugengine.3rdparty.scripting.python')


def build_bugengine(bld):
    """
        Declares the main library and entry point
    """
    bld.headers('bugengine.mak', path=bld.bugenginenode.find_node('mak'), features=['Makefile'])
    bld.headers('bugengine.kernel', [], extra_public_includes=[bld.path.make_node('bugengine/kernel/api.cpu')])
    bld.library('bugengine.minitl', bld.platforms + ['bugengine.mak', 'bugengine.kernel'])
    bld.library('bugengine.core', ['bugengine.minitl', 'bugengine.kernel'])
    bld.library('bugengine.network', ['bugengine.core'])
    bld.library('bugengine.rtti', ['bugengine.core', 'bugengine.network'], ['bugengine.3rdparty.system.zlib'])
    bld.library('bugengine.filesystem', ['bugengine.core', 'bugengine.rtti'], ['bugengine.3rdparty.system.minizip'])
    bld.library('bugengine.rttiparse', ['bugengine.core', 'bugengine.rtti', 'bugengine.filesystem'])
    bld.library('bugengine.settings', ['bugengine.rtti', 'bugengine.rttiparse'])
    bld.library('bugengine.resource', ['bugengine.core', 'bugengine.rtti', 'bugengine.filesystem'])
    bld.library('bugengine.scheduler', ['bugengine.core', 'bugengine.rtti', 'bugengine.resource', 'bugengine.settings'])
    bld.library('bugengine.world', ['bugengine.core', 'bugengine.rtti', 'bugengine.resource', 'bugengine.scheduler'])
    bld.library(
        'bugengine.plugin', ['bugengine.core', 'bugengine.rtti', 'bugengine.filesystem', 'bugengine.resource', 'bugengine.scheduler']
    )
    bld.shared_library(
        'bugengine', [
            'bugengine.core', 'bugengine.rtti', 'bugengine.rttiparse', 'bugengine.settings', 'bugengine.scheduler',
            'bugengine.filesystem', 'bugengine.world', 'bugengine.plugin'
        ],
        path=bld.path.find_node('bugengine/bugengine')
    )

    bld.engine('bugengine.launcher', ['bugengine'])


def build_plugins(bld):
    """
        Declares all plugins
    """
    bld.recurse('plugin/compute/opencl/mak/build.py')
    bld.plugin('plugin.debug.runtime', ['bugengine'])
    bld.plugin('plugin.debug.assert', ['bugengine', 'plugin.debug.runtime'])

    bld.plugin('plugin.scripting.package', ['bugengine'])

    bld.plugin('plugin.physics.bullet', ['bugengine'], ['bugengine.3rdparty.physics.bullet'])

    bld.plugin('plugin.graphics.3d', ['bugengine'])
    bld.plugin('plugin.graphics.shadermodel1', ['bugengine', 'plugin.graphics.3d'])
    bld.plugin(
        'plugin.graphics.shadermodel2', ['bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1']
    )
    bld.plugin(
        'plugin.graphics.shadermodel3',
        ['bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2']
    )
    bld.plugin(
        'plugin.graphics.shadermodel4', [
            'bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2',
            'plugin.graphics.shadermodel3'
        ]
    )

    #bld.plugin('plugin.audio.AL',
    #           ['bugengine'],
    #           ['bugengine.3rdparty.audio.OpenAL'])

    bld.plugin('plugin.scripting.lua', ['bugengine'], ['bugengine.3rdparty.scripting.lua'])
    bld.plugin('plugin.input.input', ['bugengine'])
    bld.shared_library('plugin.scripting.pythonlib', ['bugengine'], conditions=['python'])
    bld.plugin('plugin.scripting.python', ['bugengine', 'plugin.scripting.pythonlib'], conditions=['python'])
    bld.python_module(
        'py_bugengine', ['bugengine', 'plugin.scripting.pythonlib'],
        path=bld.path.find_node('plugin/scripting/pythonmodule'),
        conditions=['python']
    )
    if bld.env.PROJECTS:
        python_deps = ['bugengine.3rdparty.scripting.python%s' % version.replace('.', '') for version in bld.env.PYTHON_VERSIONS]
        bld.plugin('plugin.scripting.pythonbinding', ['bugengine', 'plugin.scripting.pythonlib'] + python_deps)
    else:
        for version in bld.env.PYTHON_VERSIONS:
            short_version = version.replace('.', '')
            bld.plugin(
                'plugin.scripting.python%s' % short_version, ['bugengine', 'plugin.scripting.python'],
                ['bugengine.3rdparty.scripting.python%s' % short_version],
                path=bld.path.find_node('plugin/scripting/pythonbinding'),
                conditions=['python%s' % version]
            )

    bld.plugin('plugin.compute.cpu', ['bugengine'], features=['bugengine:cpu:variants'])
    bld.plugin(
        'plugin.compute.opencl', ['bugengine', 'plugin.compute.cpu'], ['bugengine.3rdparty.compute.OpenCL'],
        conditions=['OpenCL'],
        extra_defines=['CL_TARGET_OPENCL_VERSION=120'],
        extra_public_defines=['CL_TARGET_OPENCL_VERSION=120']
    )
    #bld.plugin(
    #    'plugin.compute.opencl_gl',
    #    ['bugengine', 'plugin.graphics.GL4', 'plugin.compute.opencl', 'plugin.compute.cpu'],
    #    ['bugengine.3rdparty.graphics.OpenGL', 'bugengine.3rdparty.compute.OpenCL'],
    #    features=['OpenGL', 'OpenCL', 'GUI']
    #)
    bld.plugin('plugin.compute.cuda', ['bugengine'], ['bugengine.3rdparty.compute.CUDA'], conditions=['cuda'])

    bld.plugin(
        'plugin.graphics.nullrender', [
            'bugengine', 'plugin.graphics.3d', 'plugin.graphics.shadermodel1', 'plugin.graphics.shadermodel2',
            'plugin.graphics.shadermodel3', 'plugin.graphics.shadermodel4'
        ]
    )
    bld.plugin(
        'plugin.graphics.windowing', ['bugengine', 'plugin.graphics.3d'],
        ['bugengine.3rdparty.system.X11', 'bugengine.3rdparty.graphics.OpenGL'],
        conditions=['GUI']
    )
    bld.plugin(
        'plugin.graphics.GL4', ['bugengine', 'plugin.graphics.windowing'], ['bugengine.3rdparty.graphics.OpenGL'],
        conditions=['OpenGL', 'GUI']
    )
    bld.plugin(
        'plugin.graphics.Dx9', ['bugengine', 'plugin.graphics.windowing'], ['bugengine.3rdparty.graphics.DirectX9'],
        conditions=['DirectX9', 'GUI']
    )
    #bld.plugin('plugin.graphics.Dx10',
    #           ['bugengine', 'plugin.graphics.windowing'],
    #           ['bugengine.3rdparty.graphics.DirectX10'],
    #           conditions=['DirectX10', 'GUI'])
    #bld.plugin('plugin.graphics.Dx11',
    #           ['bugengine', 'plugin.graphics.windowing'],
    #           ['bugengine.3rdparty.graphics.DirectX11'],
    #           conditions=['DirectX11', 'GUI'])
    bld.plugin(
        'plugin.graphics.GLES2', ['bugengine', 'plugin.graphics.windowing'], ['bugengine.3rdparty.graphics.OpenGLES2'],
        conditions=['OpenGLES2', 'GUI']
    )

    bld.plugin(
        'plugin.graphics.text', ['bugengine', 'plugin.graphics.3d'],
        ['bugengine.3rdparty.graphics.freetype', 'bugengine.3rdparty.system.fontconfig']
    )

    if bld.env.check_gtk3 or bld.env.PROJECTS:
        bld.plugin('plugin.gui.gtk3', ['bugengine'], ['bugengine.3rdparty.gui.gtk3'])
    bld.plugin('tool.bugeditor.ui', ['bugengine'])


def build_games(bld):
    """
        Declares all games/samples/tools/autotests
    """
    bld.game(
        'bugeditor', ['bugengine', 'tool.bugeditor.ui', 'plugin.scripting.package'],
        path=bld.path.find_node('tool/bugeditor/main')
    )
    bld.game('sample.text', ['bugengine', 'plugin.scripting.package', 'plugin.graphics.3d'])
    bld.game('sample.python', ['bugengine', 'plugin.scripting.package'])
    bld.game('sample.lua', ['bugengine', 'plugin.scripting.package', 'plugin.scripting.lua'])
    bld.game('help', ['bugengine', 'plugin.scripting.package'], path=bld.path.find_node('tool/help'))
    if Options.options.tests:
        bld.game('test.settings', ['bugengine'])
        bld.game('test.compute.copy', ['bugengine', 'plugin.scripting.package'])


def build(bld):
    """
        Declares each bugengine module and their dependencies
    """
    build_externals(bld)
    build_bugengine(bld)
    build_plugins(bld)
    build_games(bld)
