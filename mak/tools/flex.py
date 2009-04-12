#!/usr/bin/env python
# encoding: utf-8
# John O'Meara, 2006
# Thomas Nagy, 2006-2008

"Flex processing"

import TaskGen

def decide_ext(self, node):
	if node.name.endswith('.ll'): return '.cc'
	else: return '.c'

TaskGen.declare_chain(
	name = 'flex',
	rule = '${FLEX} -o${TGT} ${FLEXFLAGS} ${SRC}',
	ext_in = '.l .ll',
	decider = decide_ext,
	before = 'cc cxx',
	shell=True
)

def detect(conf):
	conf.find_program('flex', var='FLEX', mandatory=True)
	v = conf.env
	v['FLEXFLAGS'] = ''

