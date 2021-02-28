"""
logical-and-expression:
      inclusive-or-expression
      logical-and-expression && inclusive-or-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_logical_and_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        logical-and-expression : inclusive-or-expression
                               | logical-and-expression OP_LAND inclusive-or-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser