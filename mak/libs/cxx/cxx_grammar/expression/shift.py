"""
shift-expression:
      additive-expression
      shift-expression << additive-expression
      shift-expression >> additive-expression
"""

from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98


@glrp.rule("shift-expression : additive-expression")
@glrp.rule("shift-expression : shift-expression '<<' additive-expression")
@glrp.rule("shift-expression : shift-expression '>>' additive-expression")
@cxx98
def p_shift_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser