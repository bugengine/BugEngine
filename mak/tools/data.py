#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import os
import sys
from waflib import Task
try:
    import cPickle
except ImportError:
    import pickle as cPickle


def scan(self):
    return ([], [])


ddf = """
%s ${DDF}
-d ${MACROS_IGNORE}
${PCH_HEADER:PCH}
--module ${PLUGIN}
--tmp ${TMPDIR}
${SRC[0].path_from(bld.bldnode)}
${TGT[0].abspath()}
${TGT[1].abspath()}
${TGT[2].abspath()}
${TGT[3].abspath()}
""" % sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan

namespace_register = 'BE_REGISTER_NAMESPACE_%d_NAMED(%s, %s)\n'


class docgen(Task.Task):
    def process_node(self, node):
        return []

    def run(self):
        for input in self.inputs:
            self.outputs += self.process_node(input)
        return 0


class nsdef(Task.Task):
    def run(self):
        seen = set(['BugEngine'])
        with open(self.outputs[0].abspath(), 'w') as namespace_file:
            pch = getattr(self, 'pch', '')
            if pch:
                namespace_file.write('#include <%s>\n' % pch)
            namespace_file.write('#include <rtti/engine/namespace.hh>\n')
            for input in self.inputs:
                with open(input.abspath(), 'rb') as in_file:
                    while True:
                        try:
                            namespace = cPickle.load(in_file)
                            if '::'.join(namespace) not in seen:
                                seen.add('::'.join(namespace))
                                line = namespace_register % (len(namespace), self.generator.env.PLUGIN,
                                                             ', '.join(namespace))
                                namespace_file.write(line)
                        except EOFError:
                            break
        return 0


@extension('.h', '.hh', '.hxx')
def datagen(self, node):
    outs = []
    out_node = self.make_bld_node('src', node.parent, '%s.cc' % node.name[:node.name.rfind('.')])
    outs.append(out_node)
    outs.append(out_node.change_ext('-instances.cc'))
    outs.append(out_node.change_ext('.doc'))
    outs.append(out_node.change_ext('.namespaces'))
    tsk = self.create_task('datagen', node, outs)
    tsk.env.DDF = self.bld.bugenginenode.find_node('mak/bin/ddf.py').abspath()
    tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/libs/cpp/macros_ignore').abspath()
    tsk.env.TMPDIR = self.bld.bldnode.parent.parent.abspath()
    tsk.path = self.bld.variant_dir
    tsk.env.PCH_HEADER = ['--pch']
    tsk.env.PCH = self.pchstop and [self.pchstop] or []
    out_node.parent.mkdir()
    tsk.dep_nodes = [self.bld.bugenginenode.find_node('mak/bin/ddf.py')]
    tsk.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/cpp').ant_glob('**/*.py')
    try:
        self.out_sources += outs[:2]
    except:
        self.out_sources = outs[:2]
    self.source.append(out_node.change_ext('.doc'))
    self.source.append(out_node.change_ext('.namespaces'))


@extension('.namespaces')
def nsgen(self, node):
    try:
        self.namespace_task.set_inputs([node])
    except AttributeError:
        out_node = self.make_bld_node('src', node.parent, 'namespace_definition.cc')
        self.out_sources.append(out_node)
        self.namespace_task = self.create_task('nsdef', [node], [out_node])


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
