#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import mak.ddf
import os
import sys

path = os.path.abspath(os.path.split(sys.argv[0])[0])

def scan(self):
	return ([], [])

def doParseData(task):
	return mak.ddf.doParse(task.inputs[0].abspath(), task.outputs[0].abspath(), path, [], ['mak/macros_ignore'], task.pch)

cls = Task.task_factory('datagen', doParseData, [], 'GREEN', ext_in='.h .hh .hxx', ext_out='.cc', before='cc cxx')
cls.scan = scan

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	try:
		tsk.pch = self.pchheader
	except AttributeError:
		tsk.pch = ''
	names = ['mak/ddf.py', 'mak/rtti.py']
	#for i in names:
	#	tsk.add_file_dependency(i)

	self.source.append(outs[0])

def configure(conf):
	pass

