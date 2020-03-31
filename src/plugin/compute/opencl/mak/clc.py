#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
from waflib.Tools import c_preproc
import os
import sys


class clc(Task.Task):
    "clc"
    run_str = '${CLC_CXX} -c -x cl ${CLC_ARCH_ST:ARCH} ${CLC_CXXFLAGS} ${CLC_FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.clb']
    scan = c_preproc.scan
    color = 'PINK'


@feature('kernel_create')
@before_method('process_source')
def cl_kernel_compile(task_gen):
    source = task_gen.kernel_source[0]
    out = task_gen.make_bld_node('obj', source.parent, source.name[:source.name.rfind('.')] + '.clb')
    t = task_gen.create_task('clc', [source], [out])


def configure(configuration_context):
    configuration_context.start_msg('Looking for clang 10+')
    v = configuration_context.env
    for c in configuration_context.compilers:
        if 'Clang' in c.NAMES:
            if c.version_number >= (10, ):
                v.CLC_CXX = c.compiler_cxx
    if v.CLC_CXX:
        v.CLC_CXXFLAGS_debug = ['-D_DEBUG']
        v.CLC_CXXFLAGS_profile = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.CLC_CXXFLAGS_final = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.CLC_CXXFLAGS = ['-std=clc++']
        v.CLC_CXX_SRC_F = ''
        v.CLC_CXX_TGT_F = ['-o']
        v.CLC_ARCH_ST = ['-arch']
        v.CLC_FRAMEWORKPATH_ST = ['-F%s']
        v.CLC_FRAMEWORK_ST = ['-framework']
        v.CLC_CPPPATH_ST = ['-I']
        v.CLC_DEFINES_ST = ['-D']
        v.CLC_CXXFLAGS_warnnone = ['-w']
        v.CLC_CXXFLAGS_warnall = [
            '-Wall', '-Wextra', '-Wno-invalid-offsetof', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual',
            '-Wstrict-aliasing'
        ]
    configuration_context.end_msg(configuration_context.env.CLC_CXX)
