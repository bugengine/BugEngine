"""
and-expression:
      equality-expression
      and-expression & equality-express
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("and-expression : equality-expression")
@glrp.rule("and-expression : and-expression '&' equality-expression")
@cxx98
def p_and_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser