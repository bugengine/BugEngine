"""
logical-or-expression:
      logical-and-expression
      logical-or-expression || logical-and-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("logical-or-expression : logical-and-expression")
@glrp.rule("logical-or-expression : logical-or-expression '||' logical-and-expression")
@cxx98
def p_logical_or_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser