#!/usr/bin/env python
# encoding: utf-8

import TaskGen

TaskGen.declare_chain(
	name = 'bin2c',
	rule = '${BIN2C} "${SRC[0].abspath()}" "${TGT[0].bldpath(env)}" ${TGT[0].file_base()}',
	ext_in = '.bin',
	ext_out = '_bin.h',
	before = 'cc cxx',
	shell=True,
	reentrant=False
)

def detect(conf):
	bin2c = conf.find_program('bin2c', var='BIN2C', mandatory=True)

