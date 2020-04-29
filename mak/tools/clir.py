#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
from waflib.Tools import c_preproc
import os
import sys

try:
    import cPickle as pickle
except ImportError:
    import pickle

template_cl = """
#include "%(kernel_source)s"

struct Parameter
{
    void* begin;
    void* end;
};

__kernel void _kmain()
{
    kmain(0, 0,
          %(args)s
    );
}
"""


class cl_trampoline(Task.Task):
    "cl_trampoline"
    color = 'PINK'

    def run(self):
        with open(self.inputs[0].abspath(), 'rb') as input_file:
            kernel_name, method, _, includes, source = pickle.load(input_file)

        args = []
        for arg in method.parameters[2:]:
            args.append((arg.name, arg.type))
        params = {
            'pch': '#include <%s>\n' % self.generator.pchstop if self.generator.pchstop else '',
            'kernel_source': source,
            'args': ',\n          '.join('%s(0, 0, 0)' % arg[1] for i, arg in enumerate(args)),
        }

        with open(self.outputs[0].abspath(), 'w') as out:
            out.write(template_cl % params)


class clc32(Task.Task):
    "clc32"
    run_str = '${CLC_CXX} -cc1 -emit-llvm -x cl -triple spir-unknown-unknown ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.ll32']
    scan = c_preproc.scan
    color = 'PINK'


class clc64(Task.Task):
    "clc64"
    run_str = '${CLC_CXX} -cc1 -emit-llvm -x cl -triple spir64-unknown-unknown ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.ll64']
    scan = c_preproc.scan
    color = 'PINK'


@feature('kernel_create')
@before_method('process_source')
def cl_ir_kernel_compile(task_gen):
    source = task_gen.kernel_source
    out_cl = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.trampoline.cl')
    out_ll_32 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.ll32')
    out_ll_64 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.ll64')

    task_gen.create_task('cl_trampoline', [task_gen.kernel_ast], [out_cl])
    task_gen.create_task('clc32', [out_cl], [out_ll_32])
    task_gen.create_task('clc64', [out_cl], [out_ll_64])
    task_gen.source += [out_ll_32, out_ll_64]


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
