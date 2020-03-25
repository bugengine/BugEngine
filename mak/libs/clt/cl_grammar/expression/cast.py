"""
cast-expression:
      unary-expression
      ( type-id ) cast-expression
"""

from be_typing import TYPE_CHECKING


def p_cast_expression(p):
    # type: (YaccProduction) -> None
    """
        cast-expression : unary-expression
                        | LPAREN type-id RPAREN cast-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction