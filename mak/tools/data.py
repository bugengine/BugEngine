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

ddf = '%s ../../../mak/ddf.py -o ${TGT[0].parent.abspath()} -D ../../../mak/cpp/macros_ignore --pch ${PCH} --namespace ${PLUGIN} -b ${str(env.BROKEN_INITIALIZER)} ${SRC[0].abspath()}' % sys.executable.replace('\\', '/')
cls = Task.task_factory('datagen', ddf, [], 'PINK', ext_in='.h .hh .hxx', ext_out='.cc', before='cxx')
cls.scan = scan


@extension('.h', '.hh', '.hxx')
def datagen(self, node):
	outs = []
	outs.append(node.change_ext('.cc'))
	outs.append(node.change_ext('-instances.cc'))
	tsk = self.create_task('datagen', node, outs)
	tsk.path = self.bld.variant_dir
	tsk.env.detach()
	if self.category == 'plugin' or self.category == 'game':
		tsk.env.PLUGIN = self.sname
	else:
		tsk.env.PLUGIN = 'game'
	try:
		tsk.env.PCH = self.pchheader
	except AttributeError:
		pass
	tsk.dep_nodes = [
		self.path.find_or_declare('mak/ddf.py'),
		self.path.find_or_declare('mak/cpp/lexer.py'),
		self.path.find_or_declare('mak/cpp/parser.py'),
		self.path.find_or_declare('mak/cpp/grammar/unit.py'),
		self.path.find_or_declare('mak/cpp/grammar/exprs.py'),
		self.path.find_or_declare('mak/cpp/grammar/namespace.py'),
		self.path.find_or_declare('mak/cpp/grammar/name.py'),
		self.path.find_or_declare('mak/cpp/grammar/using.py'),
		self.path.find_or_declare('mak/cpp/grammar/tag.py'),
		self.path.find_or_declare('mak/cpp/grammar/comment.py'),
		self.path.find_or_declare('mak/cpp/grammar/struct.py'),
		self.path.find_or_declare('mak/cpp/grammar/stream.py'),
		self.path.find_or_declare('mak/cpp/grammar/enum.py'),
		self.path.find_or_declare('mak/cpp/grammar/method.py'),
		self.path.find_or_declare('mak/cpp/grammar/variable.py'),
		self.path.find_or_declare('mak/cpp/grammar/type.py'),
		self.path.find_or_declare('mak/cpp/grammar/template.py'),
		self.path.find_or_declare('mak/cpp/grammar/keywords.py'),
		self.path.find_or_declare('mak/cpp/grammar/value.py'),
		self.path.find_or_declare('mak/cpp/grammar/skip.py'),
		]
	self.source.append(outs[0])
	self.source.append(outs[1])

def configure(conf):
	pass

