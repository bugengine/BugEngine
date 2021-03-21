"""
ctor-initializer:
      : mem-initializer-list
mem-initializer-list:
      mem-initializer ...opt     C++0x
      mem-initializer , mem-initializer-list ...opt     C++0x
mem-initializer:
      mem-initializer-id ( expression-listopt )
      mem-initializer-id braced-init-list     C++0x
mem-initializer-id:
      class-or-decltype
      identifier
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("ctor-initializer : ':' mem-initializer-list")
@cxx98
def p_ctor_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("mem-initializer-list : mem-initializer")
@glrp.rule("mem-initializer-list : mem-initializer ',' mem-initializer-list")
@cxx98
def p_mem_initializer_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("mem-initializer-list : mem-initializer '...'")
@glrp.rule("mem-initializer-list : mem-initializer ',' mem-initializer-list '...'")
@cxx11
def p_mem_initializer_list_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("mem-initializer : mem-initializer-id '(' expression-list? ')'")
@cxx98
def p_mem_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("mem-initializer : mem-initializer-id braced-init-list")
@cxx11
def p_mem_initializer_braced(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("mem-initializer-id : class-or-decltype")
@cxx98
def p_mem_initializer_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser