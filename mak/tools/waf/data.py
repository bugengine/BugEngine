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
-d ${MACROS_IGNORE}
--pch ${PCH}
--module ${PLUGIN}
--tmp ${TMPDIR}
${SRC[0].path_from(bld.bldnode)}
${TGT[0].abspath()}
${TGT[1].abspath()}
${TGT[2].abspath()}
""" % sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan


class docgen(Task.Task):
    def process_node(self, node):
        return []

    def run(self):
        for input in self.inputs:
            self.outputs += self.process_node(input)
        return 0


@extension('.h', '.hh', '.hxx')
def datagen(self, node):
    outs = []
    out_node = self.make_bld_node('src', node.parent, '%s.cc'%node.name[:node.name.rfind('.')])
    outs.append(out_node)
    outs.append(out_node.change_ext('-instances.cc'))
    outs.append(out_node.change_ext('.doc'))
    tsk = self.create_task('datagen', node, outs)
    tsk.env.DDF = self.bld.bugenginenode.find_node('mak/tools/ddf.py').abspath()
    tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/libs/cpp/macros_ignore').abspath()
    tsk.env.TMPDIR = self.bld.bldnode.parent.abspath()
    tsk.path = self.bld.variant_dir
    tsk.env.PCH = self.pchstop
    out_node.parent.mkdir()
    tsk.dep_nodes = [self.bld.bugenginenode.find_node('mak/tools/ddf.py')]
    tsk.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/cpp').ant_glob('**/*.py')
    try:
        self.out_sources += outs
    except:
        self.out_sources = outs[:]


@extension('.doc')
def docgen(self, node):
    if self.source_nodes[0].is_child_of(self.bld.bugenginenode):
        out_node = self.bld.bugenginenode
    else:
        out_node = self.bld.srcnode
    doc_task = self.create_task('docgen', [], [])
    doc_task.out_dir = out_node.make_node('doc/api')
    doc_task.out_dir.mkdir()
    doc_task.set_inputs([node])
