"""
iteration-statement:
      while ( condition ) statement
      do statement while ( expression ) ;
      for ( for-init-statement conditionopt ; expressionopt ) statement
      for ( for-range-declaration : for-range-initializer ) statement     C++0x
for-init-statement:
      expression-statement
      simple-declaration
for-range-declaration:
      attribute-specifier-seqopt type-specifier-seq declarator     C++0x
for-range-initializer:
      expression braced-init-list     C++0x
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_iteration_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        iteration-statement : KW_WHILE LPAREN condition RPAREN statement
                            | KW_DO statement KW_WHILE LPAREN expression RPAREN SEMI
                            | KW_FOR LPAREN for-init-statement condition? SEMI expression? RPAREN statement
    """


@cpp11
def p_iteration_statement_range(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        iteration-statement : KW_FOR LPAREN for-range-declaration COLON for-range-initializer RPAREN statement
    """


@cpp98
def p_for_init_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        for-init-statement : expression-statement
                           | simple-declaration
    """


@cpp11
def p_for_range_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        for-range-declaration : attribute-specifier-seq? type-specifier-seq declarator
    """


@cpp11
def p_for_range_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        for-range-initializer : expression braced-init-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser