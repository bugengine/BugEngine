#!/usr/bin/env python
# encoding: utf-8

from waflib import Task, Utils
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, taskgen_method
from waflib.Tools import c_preproc
import os
import sys


class nvcc(Task.Task):
    "nvcc"
    run_str = '${NVCC_CXX} -c ${NVCC_CXXFLAGS} ${NVCC_FRAMEWORKPATH_ST:FRAMEWORKPATH} ${NVCC_CPPPATH_ST:INCPATHS} ${NVCC_DEFINES_ST:DEFINES} -D_NVCC=1 ${NVCC_CXX_SRC_F}${SRC[0].abspath()} ${NVCC_CXX_TGT_F} ${TGT}'
    ext_out = ['.cub']
    scan = c_preproc.scan
    color = 'PINK'


@conf
def check_nvcc(configuration_context, nvcc):
    raise Exception('failed')
    if not configuration_context.check(
        code="__global__ void kernel_main() { }", features='cuda', msg='checking for cuda'
    ):
        raise Exception('failed')


@feature('kernel_create')
@before_method('process_source')
def kernel_translate_source(task_gen):
    source = task_gen.kernel_source[0]
    out = task_gen.make_bld_node('obj', source.parent, source.name[:source.name.rfind('.')] + '.cub')
    t = task_gen.create_task('nvcc', [source], [out])


def configure(configuration_context):
    compilers = []
    configuration_context.start_msg('Looking for CUDA')
    v = configuration_context.env
    environ = getattr(configuration_context, 'environ', os.environ)
    bindirs = environ['PATH'].split(os.pathsep) + v.EXTRA_PATH
    if v.HOST == 'linux':
        local_dirs = os.listdir('/usr/local')
        for local_dir in local_dirs:
            if local_dir.startswith('cuda'):
                local_dir = os.path.join('/usr/local', local_dir)
                if os.path.isdir(local_dir) and not os.path.islink(local_dir):
                    bindirs.append(os.path.join(local_dir, 'bin'))
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

    if compilers:
        v.NVCC_COMPILERS = sorted(compilers)
        v.NVCC_CXXFLAGS_debug = ['-D_DEBUG']
        v.NVCC_CXXFLAGS_profile = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.NVCC_CXXFLAGS_final = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.NVCC_CXXFLAGS = ['-c', '-x', 'cu']
        v.NVCC_CXX_SRC_F = ''
        v.NVCC_CXX_TGT_F = ['-o']
        v.NVCC_ARCH_ST = ['-arch']
        v.NVCC_FRAMEWORKPATH_ST = ['-F%s']
        v.NVCC_FRAMEWORK_ST = ['-framework']
        v.NVCC_CPPPATH_ST = ['-I']
        v.NVCC_DEFINES_ST = ['-D']
        v.NVCC_CXXFLAGS_warnnone = ['-w']
        v.NVCC_CXXFLAGS_warnall = [
            '-Wall', '-Wextra', '-Wno-invalid-offsetof', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual',
            '-Wstrict-aliasing'
        ]
    configuration_context.end_msg(
        ', '.join('.'.join(str(x) for x in p[0]) for p in configuration_context.env.NVCC_COMPILERS)
    )
