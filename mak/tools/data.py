#!/usr/bin/env python
# encoding: utf-8

import Task
from TaskGen import extension

data = 'python ../../mak/ddf.py -o ${TGT[0].bld_dir(env)} ${SRC[0].abspath()}'
cls = Task.simple_task_type('datagen', data, 'GREEN', ext_in='.h .hh .hxx', ext_out='.cc', before='c cxx')

@extension(['.h', '.hh', '.hxx'])
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	tsk.set_outputs(outs)
	# and the c/cxx file must be compiled too
	self.allnodes.append(outs[0])

def detect(conf):
	pass

