"""
and-expression:
      equality-expression
      and-expression & equality-express
"""

from be_typing import TYPE_CHECKING


def p_and_expression(p):
    # type: (YaccProduction) -> None
    """
        and-expression : equality-expression
                       | and-expression OP_AND equality-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction