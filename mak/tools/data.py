#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import mak.ddf
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

cls = Task.task_factory('datagen', doParseData, [], 'GREEN', ext_in='.h .hh .hxx', ext_out='.cc')
cls.scan = scan
old_runnable_status = cls.runnable_status
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
cls.run = run

@extension('.h', '.hh', '.hxx')
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	tsk = self.create_task('datagen', node, outs)
	tsk.path = self.bld.variant_dir
	if self.category == 'plugin':
		tsk.plugin = self.name
	else:
		tsk.plugin = 'game'
	try:
		tsk.pch = self.pchheader
	except AttributeError:
		tsk.pch = ''
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

