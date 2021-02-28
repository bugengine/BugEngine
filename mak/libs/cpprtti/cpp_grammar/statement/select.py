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

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_selection_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        selection-statement : KW_IF LPAREN condition RPAREN statement
                            | KW_IF LPAREN condition RPAREN statement KW_ELSE statement
                            | KW_SWITCH LPAREN condition RPAREN statement
    """


@cpp98
def p_condition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        condition : expression
                  | attribute-specifier-seq? decl-specifier-seq declarator OP_EQUALS initializer-clause
                  | attribute-specifier-seq? decl-specifier-seq declarator braced-init-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser