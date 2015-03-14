#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task


kernel_preprocess = '${KERNEL_PREPROCESS} ${SRC[0].abspath()} ${TGT[0].abspath()}'
cls = Task.task_factory('kernel_preprocess', kernel_preprocess, [], 'PINK')

@feature('kernel')
@before_method('process_source')
def kernel_build_preprocess(self):
    for kernel_source in getattr(self, 'kernel_source', []):
        out = self.make_bld_node('.src', None, '%s-preprocessed.cc' % (self.target))
        self.source.append(out)
        tsk = self.create_task('kernel_preprocess', [kernel_source], [out])
        tsk.path = self.bld.variant_dir
        tsk.dep_nodes = [self.bld.bugenginenode.find_or_declare('mak/kernel_preprocess.py')]

def configure(conf):
    conf.env.KERNEL_PREPROCESS = [
            sys.executable.replace('\\', '/'),
            conf.bugenginenode.find_node('mak/kernel_preprocess.py').abspath()
        ]

