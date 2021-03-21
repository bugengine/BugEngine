"""
conditional-expression:
      logical-or-expression
      logical-or-expression ? expression : assignment-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("conditional-expression : logical-or-expression")
@glrp.rule("conditional-expression : logical-or-expression '?' expression ':' assignment-expression")
@cxx98
def p_conditional_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser