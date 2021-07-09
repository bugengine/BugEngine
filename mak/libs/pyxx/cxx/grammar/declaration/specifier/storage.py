"""
storage-class-specifier:
    static
    thread_­local
    extern
    mutable
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('storage-class-specifier : "static"')
@glrp.rule('storage-class-specifier : "thread_local"')
@glrp.rule('storage-class-specifier : "extern"')
@glrp.rule('storage-class-specifier : "mutable"')
@cxx98
def storage_class_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser