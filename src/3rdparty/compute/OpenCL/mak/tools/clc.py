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

template_cpp = """
%(pch)s
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input/input.hh>
#include    <plugin/dynobjectlist.hh>

extern const unsigned char s_cldata32[];
extern const size_t s_cldata32_size;
extern const unsigned char s_cldata64[];
extern const size_t s_cldata64_size;

_BE_PLUGIN_EXPORT const unsigned char* s_clKernel32 = s_cldata32;
_BE_PLUGIN_EXPORT const unsigned long s_clKernel32Size = s_cldata32_size;

_BE_PLUGIN_EXPORT const unsigned char* s_clKernel64 = s_cldata64;
_BE_PLUGIN_EXPORT const unsigned long s_clKernel64Size = s_cldata64_size;

_BE_REGISTER_PLUGIN(BE_KERNEL_ID, BE_KERNEL_NAME);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel32);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel32Size);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel64);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel64Size);
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
        with open(self.outputs[1].abspath(), 'w') as out:
            out.write(template_cpp % params)


class clc32(Task.Task):
    "clc32"
    run_str = '${CLC_CXX} -cc1 -emit-llvm-bc -x cl -triple spir-unknown-unknown ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.bc']
    scan = c_preproc.scan
    color = 'PINK'


class clc64(Task.Task):
    "clc64"
    run_str = '${CLC_CXX} -cc1 -emit-llvm-bc -x cl -triple spir64-unknown-unknown ${CLC_CXXFLAGS} ${CLC_CPPPATH_ST:INCPATHS} ${CLC_DEFINES_ST:DEFINES} -D_CLC=1 -DBE_COMPUTE ${CLC_CXX_SRC_F}${SRC[0].abspath()} ${CLC_CXX_TGT_F} ${TGT}'
    ext_out = ['.bc']
    scan = c_preproc.scan
    color = 'PINK'

@feature('preprocess')
def create_cl_kernels(task_gen):
    internal_deps = []

    for kernel, kernel_source, kernel_ast in task_gen.kernels:
        kernel_target = '.'.join([task_gen.parent, '.'.join(kernel), 'cl'])
        for env in task_gen.bld.multiarch_envs:
            for kernel_type, toolchain in env.KERNEL_TOOLCHAINS:
                if kernel_type != 'opencl':
                    continue
                kernel_env = task_gen.bld.all_envs[toolchain]
                target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
                if target_prefix:
                    internal_deps.append(target_prefix + kernel_target)
                tgen = task_gen.bld.get_tgen_by_name(target_prefix + task_gen.parent)

                kernel_task_gen = task_gen.bld(
                    env=kernel_env.derive(),
                    bld_env=env,
                    target=target_prefix + kernel_target,
                    target_name=target_prefix + task_gen.parent,
                    kernel=kernel,
                    features=[
                        'cxx', task_gen.bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel', 'clkernel_create'
                    ],
                    extra_use=tgen.extra_use,
                    pchstop=tgen.pchstop,
                    defines=tgen.defines + [
                        'BE_BUILD_KERNEL=1',
                        'BE_KERNEL_ID=%s_%s' % (task_gen.parent.replace('.', '_'), kernel_target.replace('.', '_')),
                        'BE_KERNEL_NAME=%s' % (kernel_target),
                        'BE_KERNEL_TARGET=%s' % kernel_type,
                    ],
                    includes=tgen.includes,
                    kernel_ast=kernel_ast,
                    use=tgen.use + [target_prefix + 'plugin.compute.opencl'],
                )
                kernel_task_gen.env.PLUGIN = kernel_task_gen.env.plugin_name
        if internal_deps:
            tgt = task_gen.bld(target=kernel_target, features=['multiarch'], use=internal_deps)


@feature('clkernel_create')
@before_method('process_source')
def cl_kernel_compile(task_gen):
    source = task_gen.kernel_ast
    cl_source = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.trampoline.cl')
    cc_source = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.trampoline.cc')
    out_bc_32 = task_gen.make_bld_node('obj', source.parent, source.name[:source.name.rfind('.')] + '.32.bc')
    out_bc_64 = task_gen.make_bld_node('obj', source.parent, source.name[:source.name.rfind('.')] + '.64.bc')
    cl_cc_32 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.32.cc')
    cl_cc_64 = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.64.cc')

    task_gen.create_task('cl_trampoline', [source], [cl_source, cc_source])
    task_gen.create_task('clc32', [cl_source], [out_bc_32])
    task_gen.create_task('clc64', [cl_source], [out_bc_64])
    task_gen.create_task('bin2c', [out_bc_32], [cl_cc_32], var='cldata32')
    task_gen.create_task('bin2c', [out_bc_64], [cl_cc_64], var='cldata64')
    task_gen.source += [cl_cc_32, cl_cc_64, cc_source]
