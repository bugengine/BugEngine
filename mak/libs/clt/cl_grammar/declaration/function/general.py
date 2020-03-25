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
        function-definition : attribute-specifier-seq-opt decl-specifier-seq-opt declarator function-body
                            | attribute-specifier-seq-opt decl-specifier-seq-opt declarator OP_EQUALS KW_DEFAULT SEMI
                            | attribute-specifier-seq-opt decl-specifier-seq-opt declarator OP_EQUALS KW_DELETE SEMI
    """


def p_function_body(p):
    # type: (YaccProduction) -> None
    """
        function-body : ctor-initializer-opt compound-statement
                      | function-try-block
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction