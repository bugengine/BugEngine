#!/usr/bin/env python
# encoding: utf-8

from waflib import Task, Errors
from waflib.TaskGen import extension, feature, before_method, after_method
import os
import sys
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
--root ${ROOT_ALIAS}
--tmp ${TMPDIR}
${SRC[0].abspath()}
${LOCAL_INCLUDE}
${TGT[0].abspath()}
${TGT[1].abspath()}
${TGT[2].abspath()}
${TGT[3].abspath()}
""" % sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan

namespace_register = 'BE_REGISTER_NAMESPACE_%d_NAMED(%s, %s)\n'
namespace_alias = 'BE_REGISTER_ROOT_NAMESPACE(%s, %s, %s)\n'


class docgen(Task.Task):
    def process_node(self, node):
        return []

    def run(self):
        for input in self.inputs:
            self.outputs += self.process_node(input)
        return 0


class nsdef(Task.Task):
    def run(self):
        seen = set([])
        with open(self.outputs[0].abspath(), 'w') as namespace_file:
            pch = getattr(self, 'pch', '')
            if pch:
                namespace_file.write('#include <%s>\n' % pch)
            namespace_file.write('#include <bugengine/rtti/engine/namespace.hh>\n')
            for input in self.inputs:
                with open(input.abspath(), 'rb') as in_file:
                    while True:
                        try:
                            plugin, root_namespace, namespace = cPickle.load(in_file)
                            if (plugin, '.'.join(namespace)) not in seen:
                                root_namespace = root_namespace.split('::')
                                seen.add((plugin, '.'.join(namespace)))
                                if namespace == root_namespace:
                                    line = namespace_alias % (
                                        plugin, '_' + '_'.join(root_namespace[:-1]) if len(root_namespace) > 1 else '',
                                        root_namespace[-1]
                                    )
                                else:
                                    line = namespace_register % (len(namespace), plugin, ', '.join(namespace))
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
    for include_node in self.includes:
        if node.is_child_of(include_node):
            tsk.env.LOCAL_INCLUDE = node.path_from(include_node)
            break
    else:
        raise Errors.WafError('unable to find include root for node %s' % node)
    tsk.env.DDF = self.bld.bugenginenode.find_node('mak/tools/bin/ddf.py').abspath()
    tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/libs/cpp/macros_ignore').abspath()
    tsk.env.TMPDIR = self.bld.bldnode.parent.parent.abspath()
    tsk.path = self.bld.variant_dir
    tsk.env.PCH_HEADER = ['--pch']
    tsk.env.PCH = self.pchstop and [self.pchstop] or []
    tsk.env.ROOT_ALIAS = self.root_namespace
    out_node.parent.mkdir()
    tsk.dep_nodes = [self.bld.bugenginenode.find_node('mak/tools/bin/ddf.py')]
    tsk.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/cpp').ant_glob('**/*.py')
    try:
        self.out_sources += outs[:2]
    except:
        self.out_sources = outs[:2]
    self.source.append(out_node.change_ext('.doc'))
    self.out_sources.append(out_node.change_ext('.namespaces'))


@feature('cxxshlib', 'cshlib', 'cxxprogram', 'cprogram')
@before_method('process_source')
@after_method('static_dependencies')
def nsgen(self):
    # gather all namespaces of dependencies
    seen = set([])
    use = getattr(self, 'use', [])[:]
    while (use):
        x = use.pop()
        if x in seen:
            continue
        seen.add(x)
        try:
            y = self.bld.get_tgen_by_name(x)
        except:
            pass
        else:
            y.post()
            if 'cxxobjects' in y.features:
                use += getattr(y, 'use', [])
                for s in y.source:
                    if s.name.endswith('.namespaces'):
                        self.add_namespace_file(s)


@extension('.namespaces')
def add_namespace_file(self, node):
    if 'cobjects' in self.features:
        return
    if 'cxxobjects' in self.features:
        return
    try:
        self.namespace_task.set_inputs([node])
    except AttributeError:
        out_node = self.make_bld_node('src', None, 'namespace_definition.cc')
        self.source.append(out_node)
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
