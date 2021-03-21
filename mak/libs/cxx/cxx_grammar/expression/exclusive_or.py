"""
exclusive-or-expression:
      and-expression
      exclusive-or-expression ^ and-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("exclusive-or-expression : and-expression")
@glrp.rule("exclusive-or-expression : exclusive-or-expression '^' and-expression")
@cxx98
def p_exclusive_or_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser