"""
delete-expression:
      ::opt delete cast-expression
      ::opt delete [ ] cast-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("delete-expression : '::'? 'delete' cast-expression")
@glrp.rule("delete-expression : '::'? 'delete' '[' ']' cast-expression")
@cxx98
def p_delete_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser