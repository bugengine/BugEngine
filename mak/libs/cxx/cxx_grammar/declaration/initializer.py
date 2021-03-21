"""
initializer:
      brace-or-equal-initializer     C++0x
      ( expression-list )     C++0x
brace-or-equal-initializer:
      = initializer-clause     C++0x
      braced-init-list     C++0x
initializer-clause:
      assignment-expression     C++0x
      braced-init-list     C++0x
initializer-list:
      initializer-clause ...opt     C++0x
      initializer-list , initializer-clause ...opt     C++0x
braced-init-list:
      { initializer-list ,opt }     C++0x
      { }     C++0x
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("initializer : brace-or-equal-initializer")
@glrp.rule("initializer : '(' expression-list ')'")
@cxx98
def p_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("brace-or-equal-initializer : '=' initializer-clause")
@cxx98
def p_brace_or_equal_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("brace-or-equal-initializer : braced-init-list")
@cxx11
def p_brace_or_equal_initializer_braced_init_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("initializer-clause : assignment-expression")
@cxx98
def p_initializer_clause(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("initializer-clause : braced-init-list")
@cxx11
def p_initializer_clause_braced_init_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("initializer-list : initializer-clause")
@glrp.rule("initializer-list : initializer-list ',' initializer-clause")
@cxx98
def p_initializer_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("initializer-list : initializer-clause '...'")
@glrp.rule("initializer-list : initializer-list ',' initializer-clause '...'")
@cxx11
def p_initializer_list_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("braced-init-list : '{' initializer-list ','? '}'")
@glrp.rule("braced-init-list : '{' '}'")
@cxx11
def p_braced_init_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser