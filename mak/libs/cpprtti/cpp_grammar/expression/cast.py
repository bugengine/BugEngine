"""
cast-expression:
      unary-expression
      ( type-id ) cast-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_cast_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        cast-expression : unary-expression
                        | LPAREN type-id RPAREN cast-expression
    """


@cpp98
def p_cast_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        cast-expression : unary-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser