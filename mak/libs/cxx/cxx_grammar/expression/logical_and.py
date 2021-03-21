"""
logical-and-expression:
      inclusive-or-expression
      logical-and-expression && inclusive-or-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("logical-and-expression : inclusive-or-expression")
@glrp.rule("logical-and-expression : logical-and-expression '&&' inclusive-or-expression")
@cxx98
def p_logical_and_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser