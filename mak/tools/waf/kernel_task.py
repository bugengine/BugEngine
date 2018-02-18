#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys

def scan(self):
    return ([], [])

kernel_task = """
%s ${KERNEL}
-d ${MACROS_IGNORE}
--pch ${PCH}
--module ${PLUGIN}
--tmp ${TMPDIR}
${KERNEL_NAME}
${SRC[0].path_from(bld.bldnode)}
${TGT[0].abspath()}
${TGT[1].abspath()}
""" % sys.executable.replace('\\', '/')
cls = Task.task_factory('kernel_task', kernel_task, [], 'PINK', ext_out=['.script.hh'])
cls.scan = scan


class kernel_optim_header(Task.Task):
    color = 'PINK'
    scan = scan
    vars = ['KERNEL_OPTIM_VARIANTS']

    def run(self):
        with open(self.outputs[0].abspath(), 'w') as out:
            out.write("static const char* s_cpuVariants[] = { %s };\n"
                      "static const i32 s_cpuVariantCount = %d;\n" % (', '.join('"%s"'%o for o in self.env.KERNEL_OPTIM_VARIANTS),
                                                            len(self.env.KERNEL_OPTIM_VARIANTS)))

@feature('generate_cpu_variants')
@before_method('process_source')
def generate_cpu_variants(self):
    for kernel_name, toolchain in self.env.KERNEL_TOOLCHAINS:
        if kernel_name == 'cpu':
            env = self.bld.all_envs[toolchain]
            out_header = self.make_bld_node('include', None, 'kernel_optims.hh')
            task = self.create_task('kernel_optim_header', [], [out_header])
            task.env = env
            self.env.append_unique('INCLUDES', [out_header.parent.abspath()])


@feature('preprocess')
@before_method('process_source')
def kernel_generate(self):
    for kernel, source in getattr(self, 'kernels', []):
        mak_node = self.bld.bugenginenode.find_node('mak')
        out1 = self.make_bld_node('src/kernels', None, '%stask.cc' % (os.path.join(*kernel)))
        out2 = self.make_bld_node('src/kernels', None, '%stask.script.hh' % (os.path.join(*kernel)))
        out1.parent.mkdir()
        self.out_sources.append(out1)
        self.source.append(out2)

        tsk = self.create_task('kernel_task', [source], [out1, out2])
        tsk.env.KERNEL = self.bld.bugenginenode.find_node('mak/tools/kernel_task.py').abspath()
        tsk.env.KERNEL_NAME = '.'.join(kernel)
        tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/libs/cpp/macros_ignore').abspath(),
        tsk.env.TMPDIR = self.bld.bldnode.parent.abspath()
        tsk.env.PCH = self.pchstop
        tsk.dep_nodes = [mak_node.find_node('tools/kernel_task.py')]
        tsk.dep_nodes += mak_node.find_node('libs/cpp').ant_glob('**/*.py')
        tsk.path = self.bld.variant_dir
