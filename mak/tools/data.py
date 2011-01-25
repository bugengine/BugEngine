#!/usr/bin/env python
# encoding: utf-8

import Task
from TaskGen import extension
import mak.ddf
import os
import sys

path = os.path.abspath(os.path.split(sys.argv[0])[0])

def scan(self):
	return ([], [])

def doParseData(task):
	return mak.ddf.doParse(task.inputs[0].abspath(task.env), task.outputs[0].bldpath(task.env), path, [], ['../../mak/macros_ignore'])
cls = Task.task_type_from_func('datagen', doParseData, [], 'GREEN', ext_in='.h .hh .hxx', ext_out='.cc', before='cc cxx')
cls.scan = scan

@extension(['.h', '.hh', '.hxx'])
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	# and the c/cxx file must be compiled too
	names = ['mak/ddf.py', 'mak/rtti.py']
	for i in names:
		tsk.add_file_dependency(i)

	self.allnodes.append(outs[0])

def detect(conf):
	pass

