"""
pm-expression:
      cast-expression
      pm-expression .* cast-expression
      pm-expression ->* cast-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("pm-expression : cast-expression")
@glrp.rule("pm-expression : pm-expression '.*' cast-expression")
@glrp.rule("pm-expression : pm-expression '->*' cast-expression")
@cxx98
def p_pm_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser