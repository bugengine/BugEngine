"""
parameter-declaration-clause:
  	parameter-declaration-listopt ...opt
  	parameter-declaration-list , ...
parameter-declaration-list:
  	parameter-declaration
  	parameter-declaration-list , parameter-declaration
parameter-declaration:
  	attribute-specifier-seqopt decl-specifier-seq declarator     C++0x
  	attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause     C++0x
  	attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt     C++0x
  	attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt = initializer-clause     C++0x
"""

from ....cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_parameter_declaration_clause(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameter-declaration-clause : parameter-declaration-list COMMA ELLIPSIS
                                     | parameter-declaration-list
                                     | ELLIPSIS?
    """


@cpp11
def p_parameter_declaration_clause_texpand(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameter-declaration-clause : parameter-declaration-list ELLIPSIS
    """


@cpp98
def p_parameter_declaration_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameter-declaration-list : parameter-declaration
                                   | parameter-declaration-list COMMA parameter-declaration
    """


@cpp98
def p_parameter_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator
                              | attribute-specifier-seq? decl-specifier-seq declarator OP_EQUALS initializer-clause
                              | attribute-specifier-seq? decl-specifier-seq abstract-declarator?
                              | attribute-specifier-seq? decl-specifier-seq abstract-declarator? OP_EQUALS initializer-clause
    """


@cpp98
def p_parameter_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator
                              | attribute-specifier-seq? decl-specifier-seq abstract-declarator?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser