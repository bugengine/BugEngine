"""
logical-or-expression:
      logical-and-expression
      logical-or-expression || logical-and-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_logical_or_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        logical-or-expression : logical-and-expression
                              | logical-or-expression OP_LOR logical-and-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser