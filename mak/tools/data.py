#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import os
import sys
from waflib.Utils import threading
from waflib import Task
lock = threading.Lock()
count = 0
MAX = 1


def scan(self):
	return ([], [])

def doParseData(task):
	return mak.ddf.doParse(task.inputs[0].abspath(), task.outputs[0].abspath(), '.build', [], ['mak/macros_ignore'], task.pch, task.plugin, task.env.BROKEN_INITIALIZER)

ddf = '%s ../../../mak/ddf.py -o ${TGT[0].parent.abspath()} -D ../../../mak/macros_ignore --pch ${PCH} --namespace ${PLUGIN} -b ${str(env.BROKEN_INITIALIZER)} ${SRC[0].abspath()}' % sys.executable
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc', before='cxx')
cls.scan = scan
"""old_runnable_status = cls.runnable_status
def runnable_status(self):
	global count, lock, MAX
	ret = Task.ASK_LATER
	if count >= MAX:
		return ret
	ret = old_runnable_status(self)
	if ret == Task.RUN_ME:
		lock.acquire()
		count += 1
		lock.release()
	return ret
cls.runnable_status = runnable_status
old_run = cls.run
def run(self):
	global count, lock
	try:
		ret = old_run(self)
	finally:
		lock.acquire()
		count -= 1
		lock.release()
	return ret
cls.run = run"""

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	tsk.path = self.bld.variant_dir
	tsk.env.detach()
	if self.category == 'plugin':
		tsk.env.PLUGIN = self.name
	else:
		tsk.env.PLUGIN = 'game'
	try:
		tsk.env.PCH = self.pchheader
	except AttributeError:
		pass
	tsk.dep_nodes = [
		self.path.find_or_declare('mak/cpp/lexer.py'),
		self.path.find_or_declare('mak/cpp/parser.py'),
		self.path.find_or_declare('mak/ddf.py'),
		self.path.find_or_declare('mak/rtti.py'),]
	from waflib import Options
	if Options.options.nomaster:
		self.source.append(outs[0])

def configure(conf):
	pass

