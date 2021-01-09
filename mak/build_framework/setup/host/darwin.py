# set MacOS X specific options
# detect tools : gcc, icc, clang, llvm

import os
from be_typing import TYPE_CHECKING


def setup(conf):
    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = '/System/Library/Frameworks/OpenCL.framework/Libraries:' + environ['PATH']


if TYPE_CHECKING:
    from waflib import Configure
