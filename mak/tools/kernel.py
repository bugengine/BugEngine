#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib import Task

def scan(self):
	return ([], [])

kernel = '%s ../../../../mak/kernel.py -o ${TGT[0].abspath()} -D ../../../../mak/cpp/macros_ignore --pch ${PCH} --namespace ${PLUGIN} ${SRC[0].abspath()}' % sys.executable.replace('\\', '/')
cls = Task.task_factory('kernel', kernel, [], 'PINK', ext_out=['.script.hh'])
cls.scan = scan

@feature('*')
@before_method('process_source')
def kernel_generate(self):
	for kernel, sources in getattr(self, 'kernels', []):
		out = self.bld.bldnode.make_node(self.target).make_node('%s.script.hh' % kernel)
		self.source.append(out)
		tsk = self.create_task('kernel', sources, [out])
		tsk.path = self.bld.variant_dir
		tsk.env.PCH = self.pchstop
		tsk.dep_nodes = [
				self.path.find_or_declare('mak/kernel.py'),
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
				self.path.find_or_declare('mak/cpp/grammar/enum.py'),
				self.path.find_or_declare('mak/cpp/grammar/method.py'),
				self.path.find_or_declare('mak/cpp/grammar/variable.py'),
				self.path.find_or_declare('mak/cpp/grammar/type.py'),
				self.path.find_or_declare('mak/cpp/grammar/template.py'),
				self.path.find_or_declare('mak/cpp/grammar/keywords.py'),
				self.path.find_or_declare('mak/cpp/grammar/value.py'),
				self.path.find_or_declare('mak/cpp/grammar/skip.py'),
			]
