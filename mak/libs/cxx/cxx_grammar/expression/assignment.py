"""
assignment-expression:
      conditional-expression
      logical-or-expression assignment-operator initializer-clause     C++0x
      throw-expression
assignment-operator:
      =
      *=
      /=
      %=
      +=
      -=
      >>=
      <<=
      &=
      ^=
      |=
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("assignment-expression : conditional-expression")
@glrp.rule("assignment-expression : logical-or-expression assignment-operator initializer-clause")
@glrp.rule("assignment-expression : throw-expression")
@cxx98
def p_assignment_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("assignment-operator : '='")
@glrp.rule("assignment-operator : '*='")
@glrp.rule("assignment-operator : '/='")
@glrp.rule("assignment-operator : '%='")
@glrp.rule("assignment-operator : '+='")
@glrp.rule("assignment-operator : '-='")
@glrp.rule("assignment-operator : '>>='")
@glrp.rule("assignment-operator : '<<='")
@glrp.rule("assignment-operator : '&='")
@glrp.rule("assignment-operator : '^='")
@glrp.rule("assignment-operator : '|='")
@cxx98
def p_assignment_operator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser