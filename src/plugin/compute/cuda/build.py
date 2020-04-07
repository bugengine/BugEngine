
from waflib import Task
from waflib.TaskGen import feature, before_method, taskgen_method, extension
from waflib.Tools import c_preproc
try:
    import cPickle as pickle
except ImportError:
    import pickle

template = """
%(pch)s
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input/input.hh>
#include    <plugin/dynobjectlist.hh>
#include    <minitl/array.hh>
#include    <cuda/memorybuffer.hh>
#include    <scheduler/kernel/parameters/parameters.hh>

using namespace Kernel;

#include    "%(source)s"

struct Parameter
{
    void* begin;
    void* end;
};

__device__ void _kmain(const u32 index, const u32 total, Parameter* parameters)
{
    kmain(index, total,
          %(args)s
    );
}
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
    "Generates a CUDA trampoline to call the C++ kernel"
    color = 'PINK'

    def scan(self):
        return ([], [])

    def run(self):
        with open(self.inputs[0].abspath(), 'rb') as input_file:
            kernel_name, method, _, includes, source = pickle.load(input_file)

        args = []
        for arg in method.parameters[2:]:
            args.append((arg.name, arg.type))
        params = {
            'pch': '#include <%s>\n' % self.generator.pchstop if self.generator.pchstop else '',
            'source': source,
            'args': ',\n          '.join('%s(0, 0, 0)' % arg[1] for i, arg in enumerate(args)),
        }

        with open(self.outputs[0].abspath(), 'w') as out:
            out.write(template % params)


class bin2c(Task.Task):
    color = 'PINK'

    def scan(self):
        return ([], [])

    def run(self):
        with open(self.outputs[0].abspath(), 'w') as out:
            out.write("const char* s_cudaKernel[] = { %s };\n" % ('0'))


@extension('.cu')
def process_cuda_source(task_gen, cuda_source):
    cuda_bin = task_gen.make_bld_node('obj', cuda_source.parent, cuda_source.name[:-2] + 'fatbin')
    cuda_cc = task_gen.make_bld_node('src', cuda_source.parent, cuda_source.name[:-2] + 'cc')
    task_gen.create_task('nvcc', [cuda_source], [cuda_bin])
    task_gen.create_task('bin2c', [cuda_bin], [cuda_cc])
    task_gen.source.append(cuda_cc)


@feature('cudakernel_create')
@before_method('process_source')
def build_cuda_kernels(task_gen):
    ast = task_gen.kernel_source
    out = ast.change_ext('.cu')
    task_gen.create_task('cudac', [ast], [out])
    task_gen.source.append(out)


@feature('cudakernel_create')
@before_method('process_source')
def set_extra_nvcc_flags(self):
    for f in getattr(self, 'extra_use', []) + getattr(self, 'features', []):
        self.env.append_value('NVCC_CXXFLAGS', self.env['NVCC_CXXFLAGS_%s' % f])


@feature('preprocess')
def create_cuda_kernels(task_gen):
    internal_deps = []

    for kernel, kernel_source, kernel_ast in task_gen.kernels:
        for env in task_gen.bld.multiarch_envs:
            for kernel_type, toolchain in env.KERNEL_TOOLCHAINS:
                if kernel_type != 'cuda':
                    continue
                kernel_env = task_gen.bld.all_envs[toolchain]
                target_prefix = (env.ENV_PREFIX + '/') if env.ENV_PREFIX else ''
                kernel_target = '.'.join([task_gen.parent, '.'.join(kernel), kernel_type])
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
                        'BE_BUILD_KERNEL=1',
                        'BE_KERNEL_ID=%s_%s' % (task_gen.parent.replace('.', '_'), kernel_type),
                        'BE_KERNEL_NAME=%s.%s' % (task_gen.parent, kernel_type),
                        'BE_KERNEL_TARGET=%s' % kernel_type,
                    ],
                    includes=tgen.includes,
                    kernel_source=kernel_ast,
                    use=tgen.use + ['plugin.compute.cuda'],
                )
                kernel_task_gen.env.PLUGIN = kernel_task_gen.env.plugin_name
        if internal_deps:
            tgt = task_gen.bld(target=multiarch_target, features=['multiarch'], use=internal_deps)


def build(build_context):
    pass
