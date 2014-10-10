#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task


kernel_preprocess = '${KERNEL_PREPROCESS} ${KERNEL_PREPROCESS_OUTPUT} ${TGT[0].abspath()} ${KERNEL_PREPROCESS_OPTIONS} ${KERNEL_PREPROCESS_NAMESPACE} ${PLUGIN} ${SRC[0].abspath()}'
cls = Task.task_factory('kernel_preprocess', kernel_preprocess, [], 'PINK')

@feature('kernel')
@before_method('process_source')
def kernel_build_preprocess(self):
    for kernel_source in getattr(self, 'kernel_source', []):
        out = self.make_bld_node('.src', None, '%s-preprocessed.%s' % (self.target, self.env.KERNEL_PREPROCESS_EXTENSION))
        self.source.append(out)
        tsk = self.create_task('kernel_preprocess', [kernel_source], [out])
        tsk.path = self.bld.variant_dir
        tsk.dep_nodes = [
                self.bld.bugenginenode.find_or_declare('mak/kernel_preprocess.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/lexer.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/parser.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/unit.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/exprs.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/namespace.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/name.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/using.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/tag.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/comment.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/struct.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/enum.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/method.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/variable.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/type.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/template.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/keywords.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/value.py'),
                self.bld.bugenginenode.find_or_declare('mak/cpp/grammar/skip.py'),
            ]

def configure(conf):
    conf.env.KERNEL_PREPROCESS = [
            sys.executable.replace('\\', '/'),
            conf.bugenginenode.find_node('mak/kernel_preprocess.py').abspath()
        ]
    conf.env.KERNEL_PREPROCESS_OPTIONS = ['-D', conf.bugenginenode.find_node('mak/cpp/macros_ignore').abspath()]
    conf.env.KERNEL_PREPROCESS_EXTENSION = 'cc'
    conf.env.KERNEL_PREPROCESS_OUTPUT = '-o'
    conf.env.KERNEL_PREPROCESS_NAMESPACE = '--namespace'
