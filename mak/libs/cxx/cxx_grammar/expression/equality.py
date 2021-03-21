"""
equality-expression:
      relational-expression
      equality-expression == relational-expression
      equality-expression != relational-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("equality-expression : relational-expression")
@glrp.rule("equality-expression : equality-expression '==' relational-expression")
@glrp.rule("equality-expression : equality-expression '!=' relational-expression")
@cxx98
def p_equality_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser