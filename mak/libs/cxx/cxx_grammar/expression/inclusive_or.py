"""
inclusive-or-expression:
      exclusive-or-expression
      inclusive-or-expression | exclusive-or-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("inclusive-or-expression : exclusive-or-expression")
@glrp.rule("inclusive-or-expression : inclusive-or-expression '|' exclusive-or-expression")
@cxx98
def p_inclusive_or_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser