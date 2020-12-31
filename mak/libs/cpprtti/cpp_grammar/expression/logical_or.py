"""
logical-or-expression:
      logical-and-expression
      logical-or-expression || logical-and-expression
"""

from be_typing import TYPE_CHECKING


def p_logical_or_expression(p):
    # type: (YaccProduction) -> None
    """
        logical-or-expression : logical-and-expression
                              | logical-or-expression OP_LOR logical-and-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction