#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys


class kernel_ast(Task.Task):
    color = 'PINK'
    run_str = "${PYTHON} ${KERNEL} -d ${MACROS_IGNORE} --module ${PLUGIN} --tmp ${TMPDIR} ${KERNEL_NAME} ${SRC[0].path_from(bld.bldnode)} ${TGT[0].abspath()}"

    def scan(self):
        return ([], [])


@feature('bugengine:preprocess')
@before_method('process_source')
def kernel_generate_ast(self):
    for kernel, source, out in getattr(self, 'kernels', []):
        out.parent.mkdir()
        self.source.append(out)

        tsk = self.create_task('kernel_ast', [source], [out])
        tsk.env.KERNEL = self.bld.bugenginenode.find_node('mak/tools/bin/kernel_ast.py').abspath()
        tsk.env.KERNEL_NAME = '.'.join(kernel)
        tsk.env.MACROS_IGNORE = self.bld.bugenginenode.find_node('mak/libs/cpp/macros_ignore').abspath(),
        tsk.env.TMPDIR = self.bld.bldnode.parent.parent.abspath()
        tsk.env.PYTHON = sys.executable
        mak_node = self.bld.bugenginenode.make_node('mak')
        tsk.dep_nodes = [mak_node.find_node('tools/bin/kernel_ast.py')]
        tsk.dep_nodes += mak_node.find_node('libs/cpp').ant_glob('**/*.py')
        tsk.dep_nodes += mak_node.find_node('libs/ply').ant_glob('**/*.py')
        tsk.path = self.bld.variant_dir
