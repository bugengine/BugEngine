#!/usr/bin/env python
# encoding: utf-8

from waflib import Task, Errors
from waflib.TaskGen import feature, extension, before_method
import os
import sys
from waflib import Task

class copy(Task.Task):
    color   = 'PINK'
    def run(self):
        self.outputs[0].write(self.inputs[0].read())
    def scan(self):
        return ([], [])


class clt(Task.Task):
    color   = 'PINK'
    run_str = """%s
                 ${KERNEL_CLT}
                 --tmp ${TMPDIR}
                 ${SRC[0].path_from(bld.bldnode)}
                 ${TGT[0].abspath()}""" % sys.executable.replace('\\', '/')
    def scan(self):
        return ([], [])


@feature('cpu')
@extension('.ii')
def kernel_build_cpu_source(self, source):
    target_env = self.kernel_env
    preprocessed = self.make_bld_node('src', source.parent.make_node('cpu'), source.name[:-2]+'cc')
    t = self.create_task('clt', [source], [preprocessed])
    t.path = self.bld.variant_dir
    t.env.KERNEL_CLT = self.bld.bugenginenode.find_node('mak/tools/clt.py').abspath()
    t.env.TMPDIR = self.bld.bldnode.parent.abspath()
    t.dep_nodes = [self.bld.bugenginenode.find_or_declare('mak/tools/clt.py')]
    t.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/clt').ant_glob('**/*.py')
    t.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/ply').ant_glob('**/*.py')
    self.source.append(preprocessed)
