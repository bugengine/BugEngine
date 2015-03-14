#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task

def scan(self):
    return ([], [])

kernel = '%s ${KERNEL_PREPROCESS} ${SRC[0].abspath()} ${TGT[0].abspath()}' % sys.executable.replace('\\', '/')
cls = Task.task_factory('kernel', kernel, [], 'PINK', ext_out=['.script.hh'])
cls.scan = scan

@feature('*')
@before_method('process_source')
def kernel_generate(self):
    for kernel, sources in getattr(self, 'kernels', []):
        out = self.make_bld_node('.src', None, '%s.script.hh' % (kernel))
        self.source.append(out)
        tsk = self.create_task('kernel', sources, [out])
        tsk.env.KERNEL_PREPROCESS = self.bld.bugenginenode.find_node('mak/kernel.py').abspath()
        tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/cpp/macros_ignore').abspath(),
        tsk.path = self.bld.variant_dir
        tsk.env.PCH = self.pchstop
        mak_node = self.bld.bugenginenode.find_node('mak')
        tsk.dep_nodes = [mak_node.find_node('kernel.py')]
