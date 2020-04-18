from waflib import Options
import os


def build(build_context):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for compiler in os.listdir(build_context.path.abspath()):
        if not compilers or os.path.splitext(compiler)[0] in compilers:
            build_context.recurse(compiler)
