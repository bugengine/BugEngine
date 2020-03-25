"""
additive-expression:
      multiplicative-expression
      additive-expression + multiplicative-expression
      additive-expression - multiplicative-expression
"""

from be_typing import TYPE_CHECKING


def p_additive_expression(p):
    # type: (YaccProduction) -> None
    """
        additive-expression : multiplicative-expression
                            | additive-expression OP_PLUS multiplicative-expression
                            | additive-expression OP_MINUS multiplicative-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction