#!/usr/bin/env python
# encoding: utf-8
# John O'Meara, 2006
# Thomas Nagy 2009

"flex processing"

import Task
from TaskGen import extension

flex = '${FLEX} ${FLEXFLAGS} -o ${TGT[0].bldpath(env)} ${SRC[0].abspath()}'
cls = Task.simple_task_type('flex', flex, 'GREEN', ext_in='.l .ll', ext_out='.c .cc', before='c cxx', shell=False)

@extension(['.l', '.ll'])
def big_flex(self, node):
	outs = []
	if node.name.endswith('.ll'):
		outs.append(node.change_ext('.cc'))
	else:
		outs.append(node.change_ext('.c'))

	tsk = self.create_task('flex', node, outs)
	# and the c/cxx file must be compiled too
	self.allnodes.append(outs[0])

def detect(conf):
	flex = conf.find_program('flex', var='FLEX', mandatory=True)

