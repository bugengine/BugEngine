"""
expression-statement: 	 
      expressionopt ;
"""

from ...cpp_parser import cpp98, disabled
from be_typing import TYPE_CHECKING


@cpp98
def p_expression_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        expression-statement : attribute-specifier-seq? expression SEMI
    """


@disabled
def p_expression_statement_disabled(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        expression-statement : SEMI
    """
    # covered by empty declaration


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser