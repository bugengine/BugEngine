"""
exclusive-or-expression:
      and-expression
      exclusive-or-expression ^ and-expression
"""

from be_typing import TYPE_CHECKING


def p_exclusive_or_expression(p):
    # type: (YaccProduction) -> None
    """
        exclusive-or-expression : and-expression
                                | exclusive-or-expression OP_XOR and-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction