"""
expression:
      assignment-expression
      expression , assignment-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("expression : assignment-expression")
@glrp.rule("expression : expression ',' assignment-expression")
@cxx98
def p_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser