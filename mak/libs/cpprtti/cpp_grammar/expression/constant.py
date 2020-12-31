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


def p_constant_expression_opt(p):
    # type: (YaccProduction) -> None
    """
        constant-expression-opt : constant-expression
                                | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction