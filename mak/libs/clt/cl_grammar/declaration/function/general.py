"""
function-definition:
  	attribute-specifier-seqopt decl-specifier-seqopt declarator function-body     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = default ;     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = delete ;     C++0x
function-body:
  	ctor-initializeropt compound-statement     C++0x
  	function-try-block     C++0x
"""

from be_typing import TYPE_CHECKING


def p_function_definition(p):
    # type: (YaccProduction) -> None
    """
        function-definition : attribute-specifier-seq decl-specifier-seq declarator function-body
                            | attribute-specifier-seq decl-specifier-seq declarator OP_EQUALS KW_DEFAULT SEMI
                            | attribute-specifier-seq decl-specifier-seq declarator OP_EQUALS KW_DELETE SEMI
                            | decl-specifier-seq declarator function-body
                            | decl-specifier-seq declarator OP_EQUALS KW_DEFAULT SEMI
                            | decl-specifier-seq declarator OP_EQUALS KW_DELETE SEMI
                            | attribute-specifier-seq declarator function-body
                            | attribute-specifier-seq declarator OP_EQUALS KW_DEFAULT SEMI
                            | attribute-specifier-seq declarator OP_EQUALS KW_DELETE SEMI
                            | declarator function-body
                            | declarator OP_EQUALS KW_DEFAULT SEMI
                            | declarator OP_EQUALS KW_DELETE SEMI
    """


def p_function_body(p):
    # type: (YaccProduction) -> None
    """
        function-body : ctor-initializer-opt compound-statement
                      | function-try-block
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction