#!/usr/bin/env python
# encoding: utf-8

import TaskGen


def decide_ext(self, node):
	return ['.s', '.h']

TaskGen.declare_chain(
	name = 'grit',
	rule = '${GRIT} "${SRC[0].abspath().replace(\'.grit\', \'.png\')}" -q -fts -o"${TGT[0].bldpath(env)}" -fh',
	ext_in = '.grit',
	decider = decide_ext,
	before = 'cc cxx',
	shell=True
)

def detect(conf):
	grit = conf.find_program('grit', var='GRIT', mandatory=True)

