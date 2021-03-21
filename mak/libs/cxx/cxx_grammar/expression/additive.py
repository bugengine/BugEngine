"""
additive-expression:
      multiplicative-expression
      additive-expression + multiplicative-expression
      additive-expression - multiplicative-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("additive-expression : multiplicative-expression")
@glrp.rule("additive-expression : additive-expression '+' multiplicative-expression")
@glrp.rule("additive-expression : additive-expression '-' multiplicative-expression")
@cxx98
def p_additive_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser