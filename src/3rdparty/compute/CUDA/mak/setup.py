from waflib import Utils, Configure
import os
import sys
import shlex

ARCHS = [
    (3, 0),
    (3, 5),
    (3, 7),
    (5, 0),
    (5, 2),
    (6, 0),
    (6, 1),
    (7, 0),
    (7, 5),
    (8, 0),
]


def run_nvcc(nvcc, flags):
    try:
        p = Utils.subprocess.Popen(
            nvcc + flags, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE
        )
        out, err = p.communicate()
    except Exception as e:
        raise
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding, errors='ignore')
        if not isinstance(err, str):
            err = err.decode(sys.stderr.encoding, errors='ignore')
        if p.returncode != 0:
            raise Exception(err)
        return out, err


_CUDA_SNIPPET = """
#include <cuda_runtime_api.h>
__global__ void kernel_main()
{
}

int main(int argc, const char *argv[])
{
    cudaChooseDevice(0, 0);
    return 0;
}
"""


@Configure.conf
def check_nvcc(configuration_context, nvcc):
    source_node = configuration_context.bldnode.make_node('test.cu')
    dest_node = configuration_context.bldnode.make_node('test.obj')
    try:
        source_node.write(_CUDA_SNIPPET)
        out, err = run_nvcc(
            nvcc, configuration_context.env.NVCC_CXXFLAGS +
            ['-v', source_node.abspath(), '-o',
             dest_node.abspath(), '-arch', 'compute_30']
        )
        target_includes = None
        target_libs = None
        for line in out.split('\n') + err.split('\n'):
            if line.startswith('#$ LIBRARIES='):
                target_libs = shlex.split(line.split('=')[1].strip())
            elif line.startswith('#$ INCLUDES='):
                target_includes = shlex.split(line.split('=')[1].strip())
        if target_libs is None:
            raise Exception('could not deduce target path')
        archs = []
        for a in ARCHS:
            try:
                run_nvcc(
                    nvcc, configuration_context.env.NVCC_CXXFLAGS + ['-arch', 'compute_{0}{1}'.format(*a), '--version']
                )
            except Exception as e:
                pass
            else:
                archs.append(a)
        if not archs:
            raise Exception('no viable arch found')
        return (target_includes or [], target_libs, archs)
    finally:
        source_node.delete()


def setup(configuration_context):
    if configuration_context.env.PROJECTS:
        return
    configuration_context.start_msg_setup()
    if configuration_context.env.COMPILER_NAME == 'suncc':
        configuration_context.end_msg('not available with sunCC', color='YELLOW')
        return
    if configuration_context.env.NVCC_COMPILERS:
        cuda_available = False
        toolchain = configuration_context.env.TOOLCHAIN
        for version, compiler in configuration_context.env.NVCC_COMPILERS[::-1]:
            version = '.'.join(str(x) for x in version)
            cuda_toolchain = toolchain + '-cuda{}'.format(version)
            configuration_context.setenv(cuda_toolchain, env=configuration_context.env.detach())
            v = configuration_context.env
            v.NVCC_CXX = compiler
            cxx_compiler = v.CXX[0]
            if v.MSVC_COMPILER:
                cxx_compiler = configuration_context.find_program('cl',
                                                                  path_list=configuration_context.env.MSVC_PATH)[0]
            v.append_value('NVCC_CXXFLAGS', ['--compiler-bindir', cxx_compiler])
            if v.ARCH_LP64:
                v.append_value('NVCC_CXXFLAGS', ['-m64'])
            else:
                v.append_value('NVCC_CXXFLAGS', ['-m32'])
            carry = False
            for flag in v.CXXFLAGS:
                if flag == '-include':
                    carry = True
                elif carry:
                    v.append_value('NVCC_CXXFLAGS', ['-include', flag])
                    carry = False
                else:
                    v.append_value('NVCC_CXXFLAGS', ['-Xcompiler', flag])
            try:
                include_paths, lib_paths, archs = configuration_context.check_nvcc(compiler)
            except Exception as e:
                configuration_context.setenv(toolchain)
                del configuration_context.all_envs[cuda_toolchain]
            else:
                for a in archs:
                    v.append_value('NVCC_CXXFLAGS', ['-gencode', 'arch=compute_{0}{1},code=sm_{0}{1}'.format(*a)])
                v.append_value(
                    'NVCC_CXXFLAGS', ['-gencode', 'arch=compute_{0}{1},code=compute_{0}{1}'.format(*archs[-1])]
                )
                cuda_available = True

                configuration_context.setenv(toolchain)
                configuration_context.env.append_value('INCLUDES_cuda', include_paths)
                configuration_context.env.append_value('STLIBPATH_cuda', lib_paths)
                configuration_context.env.append_value('FEATURES', ['cuda'])
                break
        if cuda_available:
            configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cuda', cuda_toolchain)])
            configuration_context.end_msg('cuda {} [{}]'.format(version, ', '.join('{}.{}'.format(*a) for a in archs)))
        else:
            configuration_context.end_msg('not found', color='YELLOW')
