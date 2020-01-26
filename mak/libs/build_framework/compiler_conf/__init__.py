from waflib import Options, Configure
from . import clang, gcc, icc, msvc, suncc


@Configure.conf
def detect_executable(conf, program_name, path_list=[]):
    program = conf.find_program(program_name, var='TEMP_PROGRAM', path_list=path_list, mandatory=False)
    del conf.env['TEMP_PROGRAM']
    return isinstance(program, list) and program[0] or program


def options(option_context):
    gr = option_context.add_option_group('configure options')
    gr.add_option('--compilers',
                  action='store',
                  default='',
                  dest='compilers',
                  help='List of compilers to configure for')
    for compiler in clang, gcc, icc, msvc, suncc:
        compiler.options(option_context)


def configure(configuration_context):
    configuration_context.compilers = []
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for compiler in clang, gcc, icc, msvc, suncc:
        if not compilers or compiler.__name__.split('.')[-1] in compilers:
            compiler.configure(configuration_context)
    configuration_context.compilers.sort(key=lambda x: x.sort_name())
