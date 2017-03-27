#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task


kernel_preprocess = """
%s ${KERNEL_PREPROCESS}
-d ${MACROS_IGNORE}
--module ${PLUGIN}
--tmp ${TMPDIR}
${SRC[0].path_from(bld.bldnode)}
${TGT[0].abspath()}
""" % sys.executable.replace('\\', '/')
cls = Task.task_factory('kernel_preprocess', kernel_preprocess, [], 'PINK')


@feature('kernel')
@before_method('process_source')
def kernel_build_preprocess(self):
    for kernel_source in getattr(self, 'kernel_source', []):
        out = self.make_bld_node('.src', None, '%s-preprocessed.cc' % (self.target))
        self.source.append(out)
        tsk = self.create_task('kernel_preprocess', [kernel_source], [out])
        tsk.path = self.bld.variant_dir
        tsk.env.KERNEL_PREPROCESS = self.bld.bugenginenode.find_node('mak/kernel_preprocess.py').abspath()
        tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/cpp/macros_ignore').abspath()
        tsk.env.TMPDIR = self.bld.bldnode.parent.abspath()
        tsk.dep_nodes = [self.bld.bugenginenode.find_or_declare('mak/kernel_preprocess.py')]
        tsk.dep_nodes += self.bld.bugenginenode.find_node('mak/cpp').ant_glob('**/*.py')
