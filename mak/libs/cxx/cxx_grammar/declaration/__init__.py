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

from ...cxx_parser import cxx98, cxx11, disabled
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

import glrp
from be_typing import TYPE_CHECKING


@glrp.rule('declaration-seq : declaration')
@glrp.rule('declaration-seq : declaration-seq declaration')
@cxx98
def p_declaration_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('declaration : block-declaration')
@glrp.rule('declaration : function-definition')
@glrp.rule('declaration : template-declaration')
@glrp.rule('declaration : explicit-instantiation')
@glrp.rule('declaration : explicit-specialization')
@glrp.rule('declaration : linkage-specification')
@glrp.rule('declaration : namespace-definition')
@glrp.rule('declaration : empty-declaration')
@glrp.rule('declaration : attribute-declaration')
@cxx98
def p_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('block-declaration : simple-declaration')
@glrp.rule('block-declaration : asm-definition')
@glrp.rule('block-declaration : namespace-alias-definition')
@glrp.rule('block-declaration : using-declaration')
@glrp.rule('block-declaration : using-directive')
@cxx98
def p_block_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('block-declaration : static_assert-declaration')
@glrp.rule('block-declaration : alias-declaration')
@glrp.rule('block-declaration : opaque-enum-declaration')
@cxx11
def p_block_declaration_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("alias-declaration : attribute-specifier-seq? decl-specifier-seq? 'using' 'identifier' '=' type-id ';'")
@cxx11
def p_alias_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: attribute-specifier-seq?  not allowed here
    # TODO: decl-specifier-seq? empty


@glrp.rule("simple-declaration : attribute-specifier-seq? decl-specifier-seq init-declarator-list ';'")
@glrp.rule("simple-declaration : attribute-specifier-seq? init-declarator ';'")
@cxx98
def p_simple_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("empty-declaration : ';'")
@cxx98
def p_empty_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("static_assert-declaration : 'static_assert' '(' constant-expression ',' 'string-literal' ')' ';'")
@cxx11
def p_static_assert_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-declaration : attribute-specifier-seq ';'")
@cxx98
def p_attribute_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser