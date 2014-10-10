#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task

def scan(self):
    return ([], [])

kernel = '%s ${KERNEL_PREPROCESS} -o ${TGT[0].abspath()} -D ${MACROS_IGNORE} --pch ${PCH} --namespace ${PLUGIN} ${SRC[0].abspath()}' % sys.executable.replace('\\', '/')
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
        tsk.dep_nodes = [
                self.bld.bugenginenode.find_node('mak/kernel.py'),
                self.bld.bugenginenode.find_node('mak/cpp/lexer.py'),
                self.bld.bugenginenode.find_node('mak/cpp/parser.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/unit.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/exprs.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/namespace.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/name.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/using.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/tag.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/comment.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/struct.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/enum.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/method.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/variable.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/type.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/template.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/keywords.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/value.py'),
                self.bld.bugenginenode.find_node('mak/cpp/grammar/skip.py'),
            ]
