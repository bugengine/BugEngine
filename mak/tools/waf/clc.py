#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import feature, before_method
import os
import sys
from waflib.Tools import c_preproc

class cpp(Task.Task):
    "Preprocesses files"
    run_str = '${CC_CPP} ${ARCH_ST:ARCH} ${CFLAGS} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} ${CC_CPP_SRC_F}${SRC} ${CC_CPP_TGT_F}${TGT}'
    vars    = ['CCDEPS']
    ext_out = ['.i']
    scan    = c_preproc.scan
    color   = 'PINK'
