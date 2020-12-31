"""
logical-and-expression:
      inclusive-or-expression
      logical-and-expression && inclusive-or-expression
"""

from be_typing import TYPE_CHECKING


def p_logical_and_expression(p):
    # type: (YaccProduction) -> None
    """
        logical-and-expression : inclusive-or-expression
                               | logical-and-expression OP_LAND inclusive-or-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction