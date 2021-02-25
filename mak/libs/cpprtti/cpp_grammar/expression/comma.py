"""
expression:
      assignment-expression
      expression , assignment-expression
CHANGES:
    moved lambda-expression into expression to avoid constant-expression to match lambdas
"""

from .constant import *

from be_typing import TYPE_CHECKING


def p_expression(p):
    # type: (YaccProduction) -> None
    """
        expression : assignment-expression
                   | expression COMMA assignment-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction