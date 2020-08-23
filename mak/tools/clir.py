#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
from waflib.Tools import c_preproc
from waflib.Errors import WafError
import os
import sys

try:
    import cPickle as pickle
except ImportError:
    import pickle


class clc32(Task.Task):
    "clc32"
    run_str = '${CLC_CXX}  -S -emit-llvm -x cl -target spir-unknown-unknown -Xclang -finclude-default-header ${CLC_CPPPATH_ST:CLC_KERNEL_HEADER_PATH} ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.32.ll']
    color = 'PINK'

    def scan(self):
        kernel_nodes = self.generator.to_incnodes(self.generator.to_list(self.env.CLC_KERNEL_HEADER_PATH))
        self.generator.includes_nodes = kernel_nodes + self.generator.includes_nodes
        return c_preproc.scan(self)


class clc64(Task.Task):
    "clc64"
    run_str = '${CLC_CXX}  -S -emit-llvm -x cl -target spir64-unknown-unknown -Xclang -finclude-default-header ${CLC_CPPPATH_ST:CLC_KERNEL_HEADER_PATH} ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.64.ll']
    color = 'PINK'

    def scan(self):
        kernel_nodes = self.generator.to_incnodes(self.generator.to_list(self.env.CLC_KERNEL_HEADER_PATH))
        self.generator.includes_nodes = kernel_nodes + self.generator.includes_nodes
        return c_preproc.scan(self)


@feature('bugengine:kernel_create')
@before_method('process_source')
def cl_ir_kernel_compile(task_gen):
    if not task_gen.env.CLC_KERNEL_HEADER_PATH:
        raise WafError(
            'environment CLC_KERNEL_HEADER_PATH not set; make sure setup makes this variable point to the kernel header implementation for this target'
        )
    source = task_gen.kernel_source
    out_ll_32 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.32.ll')
    out_ll_64 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.64.ll')

    task_gen.create_task('clc32', [source], [out_ll_32])
    task_gen.create_task('clc64', [source], [out_ll_64])
    task_gen.source += [out_ll_32, out_ll_64]


def configure(configuration_context):
    configuration_context.start_msg('Looking for clang 10+')
    v = configuration_context.env
    for c in configuration_context.compilers:
        if 'Clang' in c.NAMES:
            if 'AppleClang' not in c.NAMES and c.version_number >= (10, ):
                v.CLC_CXX = c.compiler_cxx
            elif 'AppleClang' in c.NAMES:
                # Apple still has to release a clang10-based AppleClang compiler
                pass
    if v.CLC_CXX:
        v.CLC_CXXFLAGS_debug = ['-D_DEBUG']
        v.CLC_CXXFLAGS_profile = ['-DNDEBUG']
        v.CLC_CXXFLAGS_final = ['-DNDEBUG']
        v.CLC_CXXFLAGS = ['-O0', '-std=clc++', '-g', '-fno-rtti', '-fno-exceptions', '-fno-discard-value-names']
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
