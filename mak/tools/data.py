#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import os
import sys
from waflib import Task


def scan(self):
    return ([], [])

ddf = """
%s ${DDF}
-o ${TGT[0].parent.abspath()}
--doc ${TGT[2].abspath()}
-D ${MACROS_IGNORE}
--pch ${PCH}
--namespace ${PLUGIN}
${SRC[0].abspath()}
"""% sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan

class docgen(Task.Task):
    def process_node_gcode(self, node):
        return []

    def process_node_github(self, node):
        return []

    def cleanup_gcode(self):
        files = self.out_dir_gcode.ant_glob()
        for file in files:
            if file.name.startswith('api.'):
                if file not in self.outputs:
                    file.delete()

    def cleanup_github(self):
        files = self.out_dir_github.ant_glob()
        for file in files:
            if file not in self.outputs:
                while file:
                    file.delete()
                    file = file.parent
                    if file.children:
                        break
                    if file == self.out_dir:
                        break

    def run(self):
        for input in self.inputs:
            self.outputs += self.process_node_gcode(input)
            self.outputs += self.process_node_github(input)
        self.cleanup_gcode()
        self.cleanup_github()
        return 0

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
    outs = []
    out_node = self.make_bld_node('.src', node.parent, '%s.cc'%node.name[:node.name.rfind('.')])
    outs.append(out_node)
    outs.append(out_node.change_ext('-instances.cc'))
    outs.append(out_node.change_ext('.doc'))
    tsk = self.create_task('datagen', node, outs)
    tsk.env.DDF = self.bld.bugenginenode.find_node('mak/ddf.py').abspath()
    tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/cpp/macros_ignore').abspath()
    tsk.path = self.bld.variant_dir
    tsk.env.PCH = self.pchstop
    out_node.parent.mkdir()
    tsk.dep_nodes = [
            self.bld.bugenginenode.find_node('mak/ddf.py'),
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
        doc_task.out_dir_gcode = self.bld.srcnode.make_node('doc/wiki.googlecode')
        doc_task.out_dir_gcode.mkdir()
        doc_task.out_dir_github = self.bld.srcnode.make_node('doc/wiki.github/api')
        doc_task.out_dir_github.mkdir()
    doc_task.set_inputs([node])
