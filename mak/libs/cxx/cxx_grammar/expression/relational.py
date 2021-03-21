"""
relational-expression:
      shift-expression
      relational-expression < shift-expression
      relational-expression > shift-expression
      relational-expression <= shift-expression
      relational-expression >= shift-expression
"""

from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98


@glrp.rule("relational-expression : shift-expression")
@glrp.rule("relational-expression : relational-expression '<' shift-expression")
@glrp.rule("relational-expression : relational-expression '>' shift-expression")
@glrp.rule("relational-expression : relational-expression '<=' shift-expression")
@glrp.rule("relational-expression : relational-expression '>=' shift-expression")
@cxx98
def p_relational_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser