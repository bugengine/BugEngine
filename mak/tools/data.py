#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import os
import sys
from waflib import Task


def scan(self):
    return ([], [])

ddf = '%s ../../../../mak/ddf.py -o ${TGT[0].parent.abspath()} --doc ${TGT[2].abspath()} -D ../../../../mak/cpp/macros_ignore --pch ${PCH} --namespace ${PLUGIN} ${SRC[0].abspath()}' % sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan

class docgen(Task.Task):
    def run(self):
        return 0

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
    outs = []
    out_node = node.parent.make_node(self.target).make_node(node.name[:node.name.rfind('.')]+'.cc').get_bld()
    outs.append(out_node)
    outs.append(out_node.change_ext('-instances.cc'))
    outs.append(out_node.change_ext('.doc'))
    tsk = self.create_task('datagen', node, outs)
    tsk.path = self.bld.variant_dir
    tsk.env.PCH = self.pchstop
    out_node.parent.mkdir()
    tsk.dep_nodes = [
            self.path.find_or_declare('mak/ddf.py'),
            self.path.find_or_declare('mak/cpp/lexer.py'),
            self.path.find_or_declare('mak/cpp/parser.py'),
            self.path.find_or_declare('mak/cpp/grammar/unit.py'),
            self.path.find_or_declare('mak/cpp/grammar/exprs.py'),
            self.path.find_or_declare('mak/cpp/grammar/namespace.py'),
            self.path.find_or_declare('mak/cpp/grammar/name.py'),
            self.path.find_or_declare('mak/cpp/grammar/using.py'),
            self.path.find_or_declare('mak/cpp/grammar/tag.py'),
            self.path.find_or_declare('mak/cpp/grammar/comment.py'),
            self.path.find_or_declare('mak/cpp/grammar/struct.py'),
            self.path.find_or_declare('mak/cpp/grammar/enum.py'),
            self.path.find_or_declare('mak/cpp/grammar/method.py'),
            self.path.find_or_declare('mak/cpp/grammar/variable.py'),
            self.path.find_or_declare('mak/cpp/grammar/type.py'),
            self.path.find_or_declare('mak/cpp/grammar/template.py'),
            self.path.find_or_declare('mak/cpp/grammar/keywords.py'),
            self.path.find_or_declare('mak/cpp/grammar/value.py'),
            self.path.find_or_declare('mak/cpp/grammar/skip.py'),
        ]
    try:
        self.out_sources += outs
    except:
        self.out_sources = outs[:]

@extension('.doc')
def docgen(self, node):
    try:
        doc_task = self.bld.doc_task
    except AttributeError:
        doc_task = self.bld.doc_task = self.create_task('docgen', [], [])
    doc_task.set_inputs([node])
