"""
expression-statement: 	 
      expressionopt ;
"""

from be_typing import TYPE_CHECKING


def p_expression_statement(p):
    # type: (YaccProduction) -> None
    """
        expression-statement : expression SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction