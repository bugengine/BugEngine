#!/usr/bin/env python
# encoding: utf-8
# John O'Meara, 2006
# Thomas Nagy 2009

"flex processing"

from waflib import Task,TaskGen
import os

flex = '${FLEX} ${FLEXFLAGS} -o${TGT[0].abspath()} ${SRC[0].abspath()}'
cls = Task.task_factory('flex', flex, color='GREEN', ext_in=['.l', '.ll'], ext_out=['.c', '.cc'], before=['c', 'cxx'], shell=False)

def exec_command_flex(self, *k, **kw):
    if isinstance(k[0], list):
        lst = []
        carry = ''
        for a in k[0]:
            if a == '-o':
                    carry = a
            else:
                lst.append(carry + a)
                carry = ''
            k = [lst]
    env = os.environ.copy()
    env['PATH'] = os.path.split(k[0][0])[0]+os.pathsep+env.get('PATH', '')
    kw['env'] = env

    return self.generator.bld.exec_command(*k, **kw)

cls.exec_command = exec_command_flex

@TaskGen.extension('.l', '.ll')
def big_flex(self, node):
    outs = []
    if node.name.endswith('.ll'):
        out_node = self.make_bld_node('.src', node.parent, node.name[:-2]+'cc')
    else:
        out_node = self.make_bld_node('.src', node.parent, node.name[:-1]+'c')
    outs.append(out_node)

    tsk = self.create_task('flex', node, outs)
    try:
        self.out_sources.append(outs[0])
    except:
        self.out_sources = [outs[0]]

def configure(conf):
    flex = conf.find_program('flex', var='FLEX', mandatory=True)

