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
