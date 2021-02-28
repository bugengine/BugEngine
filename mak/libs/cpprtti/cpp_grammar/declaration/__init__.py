"""
declaration-seq:
  	declaration
  	declaration-seq declaration
declaration:
  	block-declaration
  	function-definition
  	template-declaration
  	explicit-instantiation
  	explicit-specialization
  	linkage-specification
  	namespace-definition
  	empty-declaration     C++0x
  	attribute-declaration     C++0x
block-declaration:
  	simple-declaration
  	asm-definition
  	namespace-alias-definition
  	using-declaration
  	using-directive
  	static_assert-declaration     C++0x
  	alias-declaration     C++0x
  	opaque-enum-declaration     C++0x
alias-declaration:
  	using identifier = type-id ;     C++0x
simple-declaration:
  	attribute-specifier-seqopt decl-specifier-seqopt init-declarator-listopt ;     C++0x
static_assert-declaration:
  	static_assert ( constant-expression , string-literal ) ;     C++0x
empty-declaration:
  	;     C++0x
attribute-declaration:
  	attribute-specifier-seq ;     C++0x
"""

from ...cpp_parser import cpp98
from . import specifier
from . import typedef
from . import type
from . import enum
from . import asm
from . import linkage
from . import attribute
from . import declarator
from . import name
from . import function
from . import initializer

from be_typing import TYPE_CHECKING


@cpp98
def p_declaration_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declaration-seq : declaration
                        | declaration-seq declaration
    """


@cpp98
def p_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declaration : block-declaration
                    | function-definition
                    | template-declaration
                    | explicit-instantiation
                    | explicit-specialization
                    | linkage-specification
                    | namespace-definition
    """


@cpp98
def p_block_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        block-declaration : simple-declaration
                          | asm-definition
                          | namespace-alias-definition
                          | using-declaration
                          | using-directive
                          | static_assert-declaration
                          | alias-declaration
                          | opaque-enum-declaration
    """


@cpp98
def p_alias_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        alias-declaration : attribute-specifier-seq? decl-specifier-seq? KW_USING IDENTIFIER OP_EQUALS type-id SEMI
    """
    # TODO: attribute-specifier-seq?  not allowed here
    # TODO: decl-specifier-seq? empty


@cpp98
def p_simple_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        simple-declaration : attribute-specifier-seq? decl-specifier-seq? init-declarator-list? SEMI
    """


#def p_empty_declaration(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        empty-declaration : SEMI
#    """


@cpp98
def p_static_assert_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        static_assert-declaration : KW_STATIC_ASSERT LPAREN constant-expression COMMA STRING_LITERAL RPAREN SEMI
    """


#def p_attribute_declaration(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        attribute-declaration : attribute-specifier-seq SEMI
#    """
# captured in simple-declaration

if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser