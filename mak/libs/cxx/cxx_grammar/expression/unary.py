"""
unary-expression:
      postfix-expression
      ++ cast-expression
      -- cast-expression
      unary-operator cast-expression
      sizeof unary-expression
      sizeof ( type-id )
      sizeof ... ( identifier )     C++0x
      alignof ( type-id )     C++0x
      noexcept-expression     C++0x
      new-expression
      delete-expression
unary-operator:
      *
      &
      +
      -
      !
      ~
"""

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("unary-expression : postfix-expression")
@glrp.rule("unary-expression : '++' cast-expression")
@glrp.rule("unary-expression : '--' cast-expression")
@glrp.rule("unary-expression : unary-operator cast-expression")
@glrp.rule("unary-expression : 'sizeof' unary-expression")
@glrp.rule("unary-expression : new-expression")
@glrp.rule("unary-expression : delete-expression")
@glrp.rule("unary-expression : 'sizeof' '(' type-id ')'")
@cxx98
def p_unary_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("unary-expression : 'sizeof' '...' '(' 'identifier' ')'")
@glrp.rule("unary-expression : 'alignof' '(' type-id ')'")
@glrp.rule("unary-expression : noexcept-expression")
@cxx11
def p_unary_expression_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("unary-operator : '*' | '&' | '+' | '-' | '!' | '~'")
@cxx98
def p_unary_operator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser