"""
exception-specification:
      dynamic-exception-specification     C++0x
      noexcept-specification     C++0x
dynamic-exception-specification:
      throw ( type-id-listopt )     C++0x
type-id-list:
      type-id ...opt     C++0x
      type-id-list , type-id ...opt     C++0x
noexcept-specification:
      noexcept ( constant-expression )     C++0x
      noexcept     C++0x
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("exception-specification : dynamic-exception-specification")
@cxx98
def p_exception_specification(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("exception-specification : noexcept-specification")
@cxx11
def p_exception_specification_noexcept(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("dynamic-exception-specification : 'throw' '(' type-id-list? ')'")
@cxx98
def p_dynamic_exception_specification(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("type-id-list : type-id")
@glrp.rule("type-id-list : type-id-list ',' type-id")
@cxx98
def p_type_id_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("type-id-list : type-id '...'")
@glrp.rule("type-id-list : type-id-list ',' type-id '...'")
@cxx11
def p_type_id_list_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("noexcept-specification : 'noexcept' '(' constant-expression ')'")
@cxx11
def p_noexcept_specification(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser