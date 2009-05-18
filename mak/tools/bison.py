#!/usr/bin/env python
# encoding: utf-8
# John O'Meara, 2006

"Bison processing"

import TaskGen

def decide_ext(self, node):
	c_ext = '.c'
	if node.name.endswith('.yc') or node.name.endswith('.yy'): c_ext = '.cc'
	if '-d' in self.env['BISONFLAGS']:
		return [c_ext, c_ext.replace('.cc', '.hh').replace('.c','.h')]
	else:
		return c_ext

TaskGen.declare_chain(
	name = 'bison',
	rule = 'cd ${SRC[0].bld_dir(env)} && ${BISON} ${BISONFLAGS} ${SRC[0].abspath()} -o ${TGT[0].name}',
	ext_in = '.y .yc .yy',
	decider = decide_ext,
	before = 'cc cxx',
		shell=True
)

def detect(conf):
	bison = conf.find_program('bison', var='BISON', mandatory=True)
	v = conf.env
	v['BISONFLAGS'] = '-d'

