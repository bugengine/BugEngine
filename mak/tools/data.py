#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import mak.ddf
import os
import sys

def scan(self):
	return ([], [])

def doParseData(task):
	return mak.ddf.doParse(task.inputs[0].abspath(), task.outputs[0].abspath(), '.build', [], ['mak/macros_ignore'], task.pch)

cls = Task.task_factory('datagen', doParseData, [], 'GREEN', ext_in='.h .hh .hxx', ext_out='.cc', before='cc cxx')
cls.scan = scan

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	tsk.path = self.bld.variant_dir
	try:
		tsk.pch = self.pchheader
	except AttributeError:
		tsk.pch = ''
	tsk.dep_nodes = [self.path.find_or_declare('mak/ddf.py'), self.path.find_or_declare('mak/rtti.py')]
	#self.source.append(outs[0])

def configure(conf):
	pass

