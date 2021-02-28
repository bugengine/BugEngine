"""
and-expression:
      equality-expression
      and-expression & equality-express
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_and_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        and-expression : equality-expression
                       | and-expression OP_AND equality-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser