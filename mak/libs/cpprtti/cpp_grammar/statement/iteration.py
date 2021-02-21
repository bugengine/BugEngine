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

from be_typing import TYPE_CHECKING


def p_iteration_statement(p):
    # type: (YaccProduction) -> None
    """
        iteration-statement : KW_WHILE LPAREN condition RPAREN statement
                            | KW_DO statement KW_WHILE LPAREN expression RPAREN SEMI
                            | KW_FOR LPAREN for-init-statement condition? SEMI expression? RPAREN statement
                            | KW_FOR LPAREN for-range-declaration COLON for-range-initializer RPAREN statement
    """


def p_for_init_statement(p):
    # type: (YaccProduction) -> None
    """
        for-init-statement : expression-statement
                           | simple-declaration
    """


def p_for_range_declaration(p):
    # type: (YaccProduction) -> None
    """
        for-range-declaration : attribute-specifier-seq? type-specifier-seq declarator
    """


def p_for_range_initializer(p):
    # type: (YaccProduction) -> None
    """
        for-range-initializer : expression braced-init-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction