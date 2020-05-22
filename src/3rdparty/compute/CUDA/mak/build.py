
from waflib import Task
from waflib.TaskGen import feature, before_method, taskgen_method, extension
from waflib.Tools import c_preproc
try:
    import cPickle as pickle
except ImportError:
    import pickle

template_kernel = """
_BE_PLUGIN_EXPORT void _%(kernel)s(const u32 index, const u32 total,
                              const minitl::array< minitl::weak<const BugEngine::KernelScheduler::IMemoryBuffer> >& /*argv*/)
{
    be_forceuse(index);
    be_forceuse(total);
}
_BE_REGISTER_METHOD_NAMED(BE_KERNEL_ID, _%(kernel)s, _%(kernel)s);
"""

template_cpp = """
%(pch)s
#include    <bugengine/kernel/compilers.hh>
#include    <bugengine/kernel/simd.hh>
#include    <bugengine/kernel/input/input.hh>
#include    <bugengine/plugin/dynobjectlist.hh>
#include    <bugengine/minitl/array.hh>
#include    <bugengine/plugin.compute.cuda/memorybuffer.hh>
#include    <bugengine/scheduler/kernel/parameters/parameters.hh>

using namespace Kernel;

%(kernels)s

_BE_REGISTER_PLUGIN(BE_KERNEL_ID, BE_KERNEL_NAME);
"""

class nvcc(Task.Task):
    "nvcc"
    run_str = '${NVCC_CXX} ${NVCC_CXXFLAGS} --fatbin ${NVCC_FRAMEWORKPATH_ST:FRAMEWORKPATH} ${NVCC_CPPPATH_ST:INCPATHS} -DBE_COMPUTE=1 ${NVCC_DEFINES_ST:DEFINES} -D_NVCC=1 ${NVCC_CXX_SRC_F}${SRC[0].abspath()} ${NVCC_CXX_TGT_F} ${TGT}'
    ext_out = ['.fatbin']

    def scan(self):
        try:
            incn = self.generator.includes_nodes
        except AttributeError:
            raise Errors.WafError('%r is missing a feature such as "c", "cxx" or "includes": ' % self.generator)

        nodepaths = [x for x in incn if x.is_child_of(x.ctx.srcnode) or x.is_child_of(x.ctx.bldnode)]
        nodepaths.append(self.generator.bld.bugenginenode.make_node('src/plugin/compute/cuda/api.cuda'))

        tmp = c_preproc.c_parser(nodepaths)
        tmp.start(self.inputs[0], self.env)
        return (tmp.nodes, tmp.names)

    color = 'GREEN'



class cudac(Task.Task):
    "Generates a CUDA binder to call the C++ kernel"
    color = 'PINK'

    def sig_vars(self):
        Task.Task.sig_vars(self)
        self.m.update(template_kernel.encode('utf-8'))
        self.m.update(template_cpp.encode('utf-8'))
        self.m.update((self.generator.pchstop if self.generator.pchstop else '').encode('utf-8'))

    def scan(self):
        return ([], [])

    def run(self):
        with open(self.inputs[0].abspath(), 'rb') as input_file:
            kernel_name, includes, source, kernel_methods = pickle.load(input_file)

        kernels = []
        for method, args in kernel_methods:
            args = []
            for arg in method.parameters[2:]:
                args.append((arg.name, arg.type))
            kernel_params = {
                'args': ',\n          '.join('%s(0, 0, 0)' % arg[1] for i, arg in enumerate(args)),
                'kernel': method.name
            }
            kernels.append(template_kernel % kernel_params)
        params = {
            'pch': '#include <%s>\n' % self.generator.pchstop if self.generator.pchstop else '',
            'kernels': '\n\n'.join(kernels)
        }
        with open(self.outputs[0].abspath(), 'w') as out:
            out.write(template_cpp % params)


@feature('cudakernel_create')
@before_method('process_source')
def build_cuda_kernels(task_gen):
    for f in getattr(task_gen, 'extra_use', []) + getattr(task_gen, 'features', []):
        task_gen.env.append_value('NVCC_CXXFLAGS', task_gen.env['NVCC_CXXFLAGS_%s' % f])
    ast = task_gen.kernel_ast
    cuda_source = task_gen.kernel_source
    out_cc = ast.change_ext('.cudacall.cc')
    task_gen.create_task('cudac', [ast], [out_cc])
    task_gen.source.append(out_cc)
    cuda_bin = task_gen.make_bld_node('obj', cuda_source.parent, cuda_source.name[:-2] + 'fatbin')
    cuda_cc = task_gen.make_bld_node('src', cuda_source.parent, cuda_source.name[:-2] + 'cc')
    task_gen.create_task('nvcc', [cuda_source], [cuda_bin])
    task_gen.create_task('bin2c', [cuda_bin], [cuda_cc], var='cudaKernel')
    task_gen.source.append(cuda_cc)


@feature('preprocess')
def create_cuda_kernels(task_gen):
    internal_deps = []

    for kernel, kernel_source, kernel_ast in task_gen.kernels:
        kernel_target = '.'.join([task_gen.parent, '.'.join(kernel), 'cuda'])
        for env in task_gen.bld.multiarch_envs:
            for kernel_type, toolchain in env.KERNEL_TOOLCHAINS:
                if kernel_type != 'cuda':
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
                        'cxx', task_gen.bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel', 'cudakernel_create'
                    ],
                    extra_use=tgen.extra_use,
                    pchstop=tgen.pchstop,
                    defines=tgen.defines + [
                        'BE_KERNEL_ID=%s_%s' % (task_gen.parent.replace('.', '_'), kernel_target.replace('.', '_')),
                        'BE_KERNEL_NAME=%s' % (kernel_target),
                        'BE_KERNEL_TARGET=%s' % kernel_type,
                    ],
                    includes=tgen.includes,
                    kernel_source=kernel_source,
                    kernel_ast=kernel_ast,
                    use=tgen.use + [target_prefix + 'plugin.compute.cuda'],
                    source_nodes=tgen.source_nodes,
                )
                kernel_task_gen.env.PLUGIN = kernel_task_gen.env.plugin_name
        if internal_deps:
            tgt = task_gen.bld(target=kernel_target, features=['multiarch'], use=internal_deps)

def build(bld):
    cuda = bld.thirdparty('3rdparty.compute.CUDA')
    if cuda:
        cuda.export_lib += ['cudart_static']

