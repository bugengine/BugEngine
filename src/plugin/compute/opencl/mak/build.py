from waflib import Task
from waflib.TaskGen import feature, before_method, extension
from waflib.Tools import c_preproc
import os
import sys

try:
    import cPickle as pickle
except ImportError:
    import pickle

template_cc = """
%(pch)s
#include    <bugengine/kernel/compilers.hh>
#include    <bugengine/kernel/simd.hh>
#include    <bugengine/kernel/input/input.hh>
#include    <bugengine/plugin/dynobjectlist.hh>

extern const unsigned char s_cldata32[];
extern const size_t s_cldata32_size;
extern const unsigned char s_cldata64[];
extern const size_t s_cldata64_size;

_BE_PLUGIN_EXPORT const unsigned char* s_clKernel32;
_BE_PLUGIN_EXPORT const unsigned long s_clKernel32Size;
_BE_PLUGIN_EXPORT const unsigned char* s_clKernel64;
_BE_PLUGIN_EXPORT const unsigned long s_clKernel64Size;

const unsigned char* s_clKernel32 = s_cldata32;
const unsigned long s_clKernel32Size = s_cldata32_size;
const unsigned char* s_clKernel64 = s_cldata64;
const unsigned long s_clKernel64Size = s_cldata64_size;

_BE_REGISTER_PLUGIN(BE_KERNEL_ID, BE_KERNEL_NAME);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel32);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel32Size);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel64);
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, , s_clKernel64Size);
"""


class cc_cl_trampoline(Task.Task):
    "cc_cl_trampoline"
    color = 'PINK'

    def sig_vars(self):
        self.m.update(template_cc.encode('utf-8'))
        self.m.update((self.generator.pchstop if self.generator.pchstop else '').encode('utf-8'))

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
            out.write(template_cc % params)


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
                        'cxx', task_gen.bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel', 'kernel_create',
                        'clkernel_create'
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
                    kernel_source=kernel_source,
                    kernel_ast=kernel_ast,
                    use=tgen.use + [target_prefix + 'plugin.compute.opencl'],
                )
                kernel_task_gen.env.PLUGIN = kernel_task_gen.env.plugin_name
        if internal_deps:
            tgt = task_gen.bld(target=kernel_target, features=['multiarch'], use=internal_deps)


@feature('clkernel_create')
@before_method('process_source')
def create_cc_source(task_gen):
    source = task_gen.kernel_ast
    cc_source = task_gen.make_bld_node('src', source.parent, source.name[:source.name.rfind('.')] + '.trampoline.cc')
    task_gen.create_task('cc_cl_trampoline', [source], [cc_source])
    task_gen.source += [cc_source]


@extension('.ll32', '.ll64')
def cl_kernel_compile(task_gen, source):
    if 'clkernel_create' in task_gen.features:
        ptr_size = source.name[-2:]
        cl_source = task_gen.make_bld_node(
            'src', source.parent, source.name[:source.name.rfind('.')] + '.generated.%s.cl' % ptr_size
        )
        cl_cc = task_gen.make_bld_node(
            'src', source.parent, source.name[:source.name.rfind('.')] + '.embedded.%s.cc' % ptr_size
        )

        task_gen.create_task('ircc', [source], [cl_source], ircc_target=task_gen.env.IRCC_CL_TARGET)
        task_gen.create_task('bin2c', [cl_source], [cl_cc], var='cldata%s' % ptr_size, zero_terminate=True)
        task_gen.source += [cl_cc]


def build(build_context):
    build_context.env.IRCC_CL_TARGET = build_context.path.find_node('ir2cl').abspath()