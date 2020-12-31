"""
multiplicative-expression:
      pm-expression
      multiplicative-expression * pm-expression
      multiplicative-expression / pm-expression
      multiplicative-expression % pm-expression
"""

from be_typing import TYPE_CHECKING


def p_multiplicative_expression(p):
    # type: (YaccProduction) -> None
    """
        multiplicative-expression : pm-expression
                                  | multiplicative-expression OP_TIMES pm-expression
                                  | multiplicative-expression OP_DIVIDE pm-expression
                                  | multiplicative-expression OP_MOD pm-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction