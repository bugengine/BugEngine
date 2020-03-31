from waflib.Logs import pprint


def configure(configuration_context):
    configuration_context.load('nvcc', tooldir=[configuration_context.path.make_node('mak').abspath()])


def setup(configuration_context):
    if configuration_context.env.NVCC_COMPILERS:
        configuration_context.start_msg('      `- [cuda]')
        toolchain = configuration_context.env.TOOLCHAIN
        for version, compiler in configuration_context.env.NVCC_COMPILERS:
            version = '.'.join(str(x) for x in version)
            configuration_context.setenv(toolchain + '-cuda{}'.format(version), env=configuration_context.env)
            v = configuration_context.env
            v.NVCC_CXX = compiler
            v.append_value('NVCC_CXXFLAGS', ['--compiler-bindir', v.CXX[0]])
            try:
                configuration_context.check_nvcc(compiler)
            except Exception as e:
                pprint('YELLOW', '-{}'.format(version), sep=' ')
            else:
                pprint('GREEN', '+{}'.format(version), sep=' ')
            finally:
                configuration_context.setenv(toolchain)
        configuration_context.end_msg(' ')
        configuration_context.env.append_value('NVCC_CXXFLAGS', ['--compiler-bindir', configuration_context.env.CXX])
        configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cuda', configuration_context.env.TOOLCHAIN)])
