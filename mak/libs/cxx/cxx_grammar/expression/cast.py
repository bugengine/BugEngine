"""
cast-expression:
      unary-expression
      ( type-id ) cast-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("cast-expression : unary-expression")
@glrp.rule("cast-expression : '(' type-id ')' cast-expression")
@cxx98
def p_cast_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser