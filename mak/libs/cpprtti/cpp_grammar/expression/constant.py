"""
constant-expression:
      conditional-expression
"""

from be_typing import TYPE_CHECKING


def p_constant_expression(p):
    # type: (YaccProduction) -> None
    """
        constant-expression : conditional-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction