"""
inclusive-or-expression:
      exclusive-or-expression
      inclusive-or-expression | exclusive-or-expression
"""

from be_typing import TYPE_CHECKING


def p_inclusive_or_expression(p):
    # type: (YaccProduction) -> None
    """
        inclusive-or-expression : exclusive-or-expression
                                | inclusive-or-expression OP_OR exclusive-or-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction