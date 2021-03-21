"""
multiplicative-expression:
      pm-expression
      multiplicative-expression * pm-expression
      multiplicative-expression / pm-expression
      multiplicative-expression % pm-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("multiplicative-expression : pm-expression")
@glrp.rule("multiplicative-expression : multiplicative-expression '*' pm-expression")
@glrp.rule("multiplicative-expression : multiplicative-expression '/' pm-expression")
@glrp.rule("multiplicative-expression : multiplicative-expression '%' pm-expression")
@cxx98
def p_multiplicative_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser