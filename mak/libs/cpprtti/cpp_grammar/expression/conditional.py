"""
conditional-expression:
      logical-or-expression
      logical-or-expression ? expression : assignment-expression
"""

from be_typing import TYPE_CHECKING


def p_conditional_expression(p):
    # type: (YaccProduction) -> None
    """
        conditional-expression : logical-or-expression
                               | logical-or-expression OP_COND expression COLON assignment-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction