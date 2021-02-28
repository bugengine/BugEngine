"""
conditional-expression:
      logical-or-expression
      logical-or-expression ? expression : assignment-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_conditional_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        conditional-expression : logical-or-expression
                               | logical-or-expression OP_COND expression COLON assignment-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser