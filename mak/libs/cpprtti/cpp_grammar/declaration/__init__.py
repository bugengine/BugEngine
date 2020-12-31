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

from .specifier import *
from .typedef import *
from .type import *
from .enum import *
from .asm import *
from .linkage import *
from .attribute import *
from .declarator import *
from .name import *
from .function import *
from .initializer import *

from be_typing import TYPE_CHECKING


def p_declaration_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        declaration-seq-opt : declaration-seq
    """
    # TODO: empty


def p_declaration_seq(p):
    # type: (YaccProduction) -> None
    """
        declaration-seq : declaration
                        | declaration-seq declaration
    """


def p_declaration(p):
    # type: (YaccProduction) -> None
    """
        declaration : block-declaration
                    | function-definition
                    | template-declaration
                    | explicit-instantiation
                    | explicit-specialization
                    | linkage-specification
                    | namespace-definition
                    | attribute-declaration
                    | empty-declaration
    """


def p_block_declaration(p):
    # type: (YaccProduction) -> None
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


def p_alias_declaration(p):
    # type: (YaccProduction) -> None
    """
        alias-declaration : KW_USING identifier OP_EQUALS type-id SEMI
    """


def p_simple_declaration(p):
    # type: (YaccProduction) -> None
    """
        simple-declaration : attribute-specifier-seq decl-specifier-seq init-declarator-list-opt SEMI
                           | decl-specifier-seq init-declarator-list-opt SEMI
    """


def p_empty_declaration(p):
    # type: (YaccProduction) -> None
    """
        empty-declaration : SEMI
    """


def p_static_assert_declaration(p):
    # type: (YaccProduction) -> None
    """
        static_assert-declaration : KW_STATIC_ASSERT LPAREN constant-expression COMMA STRING_LITERAL RPAREN SEMI
    """


def p_attribute_declaration(p):
    # type: (YaccProduction) -> None
    """
        attribute-declaration : attribute-specifier-seq SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction