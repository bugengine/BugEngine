"""
storage-class-specifier:
      auto     Removed in C++0x
      register
      static
      thread_local     C++0x
      extern
      mutable
"""

from ....cxx_parser import cxx98, cxx11, deprecate_cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("storage-class-specifier : 'register' | 'static' | 'extern' | 'mutable'")
@cxx98
def p_storage_class_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("storage-class-specifier : 'auto'")
@cxx98
@deprecate_cxx11
def p_storage_class_specifier_auto(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("storage-class-specifier : 'thread_local'")
@cxx11
def p_storage_class_specifier_thread_local(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser