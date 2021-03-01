"""
expression-statement: 	 
      expressionopt ;
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_expression_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        expression-statement : expression? SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser