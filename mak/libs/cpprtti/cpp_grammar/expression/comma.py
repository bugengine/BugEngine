"""
expression:
      assignment-expression
      expression , assignment-expression
CHANGES:
    moved lambda-expression into expression to avoid constant-expression to match lambdas
"""

from ...cpp_parser import cpp98

from be_typing import TYPE_CHECKING


@cpp98
def p_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        expression : assignment-expression
                   | expression COMMA assignment-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser