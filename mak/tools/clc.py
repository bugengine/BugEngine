#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
import os
import sys
from waflib.Tools import c_preproc


class cpp(Task.Task):
    "Preprocesses files"
    run_str = '${CC_CPP} ${ARCH_ST:ARCH} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} -D_CLC=1 -D__attribute__(x)= ${CC_CPP_SRC_F}${SRC[0].abspath()} ${CC_CPP_TGT_F}${TGT}'
    ext_out = ['.i']
    scan = c_preproc.scan
    color = 'PINK'


@extension('.cl')
def kernel_preprocess_source(self, source):
    name = self.target_name
    target_source = self.make_bld_node('src', source.parent, source.name[:-2] + 'ii')
    t = self.create_task('cpp', [source], [target_source])
    try:
        self.preprocessed_tasks.append(t)
    except AttributeError:
        self.preprocessed_tasks = [t]
    self.source.append(target_source)