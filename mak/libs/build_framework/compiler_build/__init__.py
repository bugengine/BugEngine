from waflib import Options
from . import clang, gcc, icc, msvc, suncc


def build(build_context):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for compiler in clang, gcc, icc, msvc, suncc:
        if not compilers or compiler.__name__.split('.')[-1] in compilers:
            compiler.build(build_context)
