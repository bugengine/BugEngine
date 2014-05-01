#!/usr/bin/env python
# encoding: utf-8
# John O'Meara, 2006
# Thomas Nagy 2009

"Bison processing"

from waflib import Task,TaskGen
import os

bison = '${BISON} ${BISONFLAGS} ${SRC[0].abspath()} -o ${TGT[0].name}'
cls = Task.task_factory('bison', bison, color='GREEN', ext_in=['.yc', '.y', '.yy'], ext_out='.cxx .h', before='c cxx flex')

def post_run_bison(task):
    source = task.outputs[0]
    header = task.outputs[1]
    env = task.env
    try:
        os.stat(header.abspath())
    except OSError:
        try:
            oldheader = source.change_ext(source.suffix()+'.h')
            os.rename(oldheader.abspath(), header.abspath())
        except OSError:
            pass
    task.post_run_orig()

cls.post_run_orig = cls.post_run
cls.post_run = post_run_bison

@TaskGen.extension('.y', '.yc', '.yy')
def big_bison(self, node):
    """when it becomes complicated (unlike flex), the old recipes work better (cwd)"""
    has_h = '-d' in self.env['BISONFLAGS']

    outs = []
    if node.name.endswith('.yc') or node.name.endswith('.yy'):
        out_node = node.parent.make_node(self.target).make_node(node.name[:-2]+'cc').get_bld()
        outs.append(out_node)
        if has_h:
            outs.append(out_node.change_ext('.hh'))
    else:
        out_node = node.parent.make_node(self.target).make_node(node.name[:-1]+'c').get_bld()
        outs.append(out_node)
        if has_h:
            outs.append(out_node.change_ext('.h'))

    tsk = self.create_task('bison', node, outs)
    tsk.cwd = out_node.parent.abspath()
    out_node.parent.mkdir()

    # and the c/cxx file must be compiled too
    try:
        self.out_sources.append(outs[0])
    except:
        self.out_sources = [outs[0]]

def configure(conf):
    bison = conf.find_program('bison', var='BISON', mandatory=True)
    v = conf.env
    v['BISONFLAGS'] = '-d'

