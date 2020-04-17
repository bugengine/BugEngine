from waflib import Utils
from waflib.Logs import pprint
from waflib.Configure import conf
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
]

def run_nvcc(nvcc, flags):
    try:
        p = Utils.subprocess.Popen(
            nvcc + flags,
            stdin=Utils.subprocess.PIPE,
            stdout=Utils.subprocess.PIPE,
            stderr=Utils.subprocess.PIPE
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

@conf
def check_nvcc(configuration_context, nvcc):
    source_node = configuration_context.bldnode.make_node('test.cu')
    dest_node = configuration_context.bldnode.make_node('test.obj')
    try:
        source_node.write(_CUDA_SNIPPET)
        out, err = run_nvcc(nvcc, configuration_context.env.NVCC_CXXFLAGS + ['-v', source_node.abspath(), '-o', dest_node.abspath(), '-arch', 'compute_30'])
        target_includes = None
        target_libs = None
        for line in out.split('\n') + err.split('\n'):
            if line.startswith('#$ LIBRARIES='):
                target_libs = shlex.split(line.split('=')[1].strip())
            elif line.startswith('#$ INCLUDES='):
                target_includes = shlex.split(line.split('=')[1].strip())
        if target_includes is None or target_libs is None:
            raise Exception('could not deduce target path')
        archs = []
        for a in ARCHS:
            try:
                run_nvcc(nvcc, configuration_context.env.NVCC_CXXFLAGS + [
                    '-arch', 'compute_{0}{1}'.format(*a), '--version'
                ])
            except Exception as e:
                pass
            else:
                archs.append(a)
        if not archs:
            raise Exception('no viable arch found')
        return (target_includes, target_libs, archs)
    finally:
        source_node.delete()


def find_cuda_registry_paths(all_versions_key):
    bindirs = []
    try:
        root_path, type = Utils.winreg.QueryValueEx(all_versions_key, 'RootInstallDir')
    except OSError:
        root_path = ''
    index = 0
    while 1:
        try:
            version = Utils.winreg.EnumKey(all_versions_key, index)
            version_key = Utils.winreg.OpenKey(all_versions_key, version)
        except OSError:
            break
        index += 1
        try:
            full_path, type = Utils.winreg.QueryValueEx(version_key, 'InstallDir')
        except OSError:
            full_path = os.path.join(root_path, version)
        bindirs.append(os.path.join(full_path, 'bin'))
    return bindirs


def find_cuda_paths(configuration_context):
    compilers = []
    v = configuration_context.env
    environ = getattr(configuration_context, 'environ', os.environ)
    bindirs = environ['PATH'].split(os.pathsep) + v.EXTRA_PATH
    if Utils.winreg:
        try:
            all_versions = Utils.winreg.OpenKey(
                Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Wow6432node\NVIDIA Corporation\GPU Computing Toolkit\CUDA'
            )
        except OSError:
            pass
        else:
            bindirs += find_cuda_registry_paths(all_versions)
        try:
            all_versions = Utils.winreg.OpenKey(
                Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\NVIDIA Corporation\GPU Computing Toolkit\CUDA'
            )
        except OSError:
            pass
        else:
            bindirs += find_cuda_registry_paths(all_versions)
    else:
        try:
            developer_dirs = os.listdir('/Developer/NVIDIA')
        except OSError:
            local_dirs = os.listdir('/usr/local')
            for local_dir in local_dirs:
                if local_dir.startswith('cuda'):
                    local_dir = os.path.join('/usr/local', local_dir)
                    if os.path.isdir(local_dir) and not os.path.islink(local_dir):
                        bindirs.append(os.path.join(local_dir, 'bin'))
        else:
            for cuda_dir in developer_dirs:
                cuda_dir = os.path.join('/Developer/NVIDIA', cuda_dir)
                if os.path.isdir(cuda_dir) and not os.path.islink(cuda_dir):
                    bindirs.append(os.path.join(cuda_dir, 'bin'))
    paths = [os.path.normpath(path) for path in bindirs]
    for path in paths:
        nvcc = configuration_context.find_program('nvcc', mandatory=False, path_list=[path], var='NVCC_TEMP_PROG')
        del v['NVCC_TEMP_PROG']
        if nvcc:
            try:
                p = Utils.subprocess.Popen(
                    nvcc + ['--version'],
                    stdin=Utils.subprocess.PIPE,
                    stdout=Utils.subprocess.PIPE,
                    stderr=Utils.subprocess.PIPE
                )
                out, err = p.communicate()
            except Exception as e:
                #print(e)
                pass
            else:
                if p.returncode == 0:
                    if not isinstance(out, str):
                        out = out.decode(sys.stdout.encoding, errors='ignore')
                    for line in out.split('\n'):
                        if line.startswith('Cuda compilation tools'):
                            version = line.split(',')[1].split()[1]
                            version = tuple(int(x) for x in version.split('.'))
                            compilers.append((version, nvcc))
    return compilers


def configure(configuration_context):
    configuration_context.start_msg('Looking for CUDA')
    v = configuration_context.env
    compilers = find_cuda_paths(configuration_context)
    if compilers:
        v.NVCC_COMPILERS = sorted(compilers)
        v.NVCC_CXXFLAGS_debug = ['-D_DEBUG', '--generate-line-info']
        v.NVCC_CXXFLAGS_profile = ['-DNDEBUG', '-O2', '--generate-line-info']
        v.NVCC_CXXFLAGS_final = ['-DNDEBUG', '-O2']
        v.NVCC_CXXFLAGS = [
            '-c', '-x', 'cu', '-I%s' % configuration_context.path.make_node('api.cuda').abspath()
        ]
        v.NVCC_CXX_SRC_F = ''
        v.NVCC_CXX_TGT_F = ['-o']
        v.NVCC_ARCH_ST = ['-arch']
        v.NVCC_FRAMEWORKPATH_ST = '-F%s'
        v.NVCC_FRAMEWORK_ST = ['-framework']
        v.NVCC_CPPPATH_ST = '-I%s'
        v.NVCC_DEFINES_ST = '-D%s'

    configuration_context.end_msg(
        ', '.join('.'.join(str(x) for x in p[0]) for p in configuration_context.env.NVCC_COMPILERS)
    )


def setup(configuration_context):
    if configuration_context.env.COMPILER_NAME == 'suncc':
        return
    if configuration_context.env.NVCC_COMPILERS:
        cuda_available = False
        configuration_context.start_msg('      `- [cuda]')
        toolchain = configuration_context.env.TOOLCHAIN
        for version, compiler in configuration_context.env.NVCC_COMPILERS[::-1]:
            version = '.'.join(str(x) for x in version)
            cuda_toolchain = toolchain + '-cuda{}'.format(version)
            configuration_context.setenv(cuda_toolchain, env=configuration_context.env)
            v = configuration_context.env
            v.NVCC_CXX = compiler
            cxx_compiler = v.CXX[0]
            if v.MSVC_COMPILER:
                cxx_compiler = configuration_context.find_program('cl', path_list=configuration_context.env.MSVC_PATH)[0]
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
                #print(e)
                #pprint('YELLOW', '-{}'.format(version), sep=' ')
                configuration_context.setenv(toolchain)
            else:
                for a in archs:
                    v.append_value('NVCC_CXXFLAGS', ['-gencode', 'arch=compute_{0}{1},code=sm_{0}{1}'.format(*a)])
                v.append_value('NVCC_CXXFLAGS', ['-gencode', 'arch=compute_{0}{1},code=compute_{0}{1}'.format(*archs[-1])])
                cuda_available = True

                configuration_context.setenv(toolchain)
                configuration_context.env.append_value('INCLUDES_cuda', include_paths)
                configuration_context.env.append_value('STLIBPATH_cuda', lib_paths)
                configuration_context.env.append_value('FEATURES', ['cuda'])
                break
        if cuda_available:
            configuration_context.env.append_value('KERNEL_TOOLCHAINS', [('cuda', cuda_toolchain)])
            configuration_context.env.append_value('NVCC_CXXFLAGS', ['--compiler-bindir', configuration_context.env.CXX])
            configuration_context.end_msg('{} [{}]'.format(version, ', '.join('{}.{}'.format(*a) for a in archs)))
        else:
            configuration_context.end_msg(None)
