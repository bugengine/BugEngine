"""
shift-expression:
      additive-expression
      shift-expression << additive-expression
      shift-expression >> additive-expression
"""

from be_typing import TYPE_CHECKING


def p_shift_expression(p):
    # type: (YaccProduction) -> None
    """
        shift-expression : additive-expression
                         | shift-expression OP_LSHIFT additive-expression
                         | shift-expression OP_RSHIFT additive-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction