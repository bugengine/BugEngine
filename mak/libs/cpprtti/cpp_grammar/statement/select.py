"""
selection-statement:
      if ( condition ) statement
      if ( condition ) statement else statement
      switch ( condition ) statement
condition:
      expression
      attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause     C++0x
      attribute-specifier-seqopt decl-specifier-seq declarator braced-init-list     C++0x
"""

from be_typing import TYPE_CHECKING


def p_selection_statement(p):
    # type: (YaccProduction) -> None
    """
        selection-statement : KW_IF LPAREN condition RPAREN statement
                            | KW_IF LPAREN condition RPAREN statement KW_ELSE statement
                            | KW_SWITCH LPAREN condition RPAREN statement
    """


def p_condition(p):
    # type: (YaccProduction) -> None
    """
        condition : expression
                  | attribute-specifier-seq? decl-specifier-seq declarator OP_EQUALS initializer-clause
                  | attribute-specifier-seq? decl-specifier-seq declarator braced-init-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction