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

from be_typing import TYPE_CHECKING


def p_parameter_declaration_clause(p):
    # type: (YaccProduction) -> None
    """
        parameter-declaration-clause : parameter-declaration-list-opt ellipsis-opt
                                     | parameter-declaration-list COMMA ELLIPSIS
    """


def p_parameter_declaration_list(p):
    # type: (YaccProduction) -> None
    """
        parameter-declaration-list : parameter-declaration
                                   | parameter-declaration-list COMMA parameter-declaration
    """


def p_parameter_declaration_list_opt(p):
    # type: (YaccProduction) -> None
    """
        parameter-declaration-list-opt : parameter-declaration-list
    """
    # TODO: empty


def p_parameter_declaration(p):
    # type: (YaccProduction) -> None
    """
        parameter-declaration : attribute-specifier-seq-opt decl-specifier-seq declarator
                              | attribute-specifier-seq-opt decl-specifier-seq declarator OP_EQUALS initializer-clause
                              | attribute-specifier-seq-opt decl-specifier-seq abstract-declarator-opt
                              | attribute-specifier-seq-opt decl-specifier-seq abstract-declarator-opt OP_EQUALS initializer-clause
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction