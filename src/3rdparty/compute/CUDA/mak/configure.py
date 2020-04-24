from waflib import Utils
import os
import sys
import shlex


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
            '-c', '-x', 'cu', '-I%s' % configuration_context.path.parent.make_node('api.cuda').abspath()
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

