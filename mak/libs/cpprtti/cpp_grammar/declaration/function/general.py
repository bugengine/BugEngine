"""
function-definition:
  	attribute-specifier-seqopt decl-specifier-seqopt declarator function-body     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = default ;     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = delete ;     C++0x
function-body:
  	ctor-initializeropt compound-statement     C++0x
  	function-try-block     C++0x
"""

from ....cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_function_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        function-definition : attribute-specifier-seq? decl-specifier-seq? declarator function-body
    """


@cpp11
def p_function_definition_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        function-definition : attribute-specifier-seq? decl-specifier-seq? declarator OP_EQUALS KW_DEFAULT SEMI
                            | attribute-specifier-seq? decl-specifier-seq? declarator OP_EQUALS KW_DELETE SEMI
    """


@cpp98
def p_function_body(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        function-body : ctor-initializer? compound-statement
                      | function-try-block
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser