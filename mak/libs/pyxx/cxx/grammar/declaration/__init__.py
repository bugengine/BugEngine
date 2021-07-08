"""
declaration-seq:
    declaration
    declaration-seq declaration

declaration:
    block-declaration
    nodeclspec-function-declaration
    function-definition
    template-declaration
    deduction-guide
    explicit-instantiation
    explicit-specialization
    export-declaration
    linkage-specification
    namespace-definition
    empty-declaration
    attribute-declaration
    module-import-declaration

block-declaration:
    simple-declaration
    asm-declaration
    namespace-alias-definition
    using-declaration
    using-enum-declaration
    using-directive
    static_assert-declaration
    alias-declaration
    opaque-enum-declaration

nodeclspec-function-declaration:
    attribute-specifier-seq? declarator ;

alias-declaration:
    using identifier attribute-specifier-seq? = defining-type-id ;

simple-declaration:
    decl-specifier-seq init-declarator-list? ;
    attribute-specifier-seq decl-specifier-seq init-declarator-list ;
    attribute-specifier-seq? decl-specifier-seq ref-qualifier? [ identifier-list ] initializer ;

static_assert-declaration:
    static_­assert ( constant-expression ) ;
    static_­assert ( constant-expression , string-literal ) ;

empty-declaration:
    ;

attribute-declaration:
    attribute-specifier-seq ;
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import specifier
from . import declarator
from . import initializer
from . import function
from . import enumeration
from . import namespace
from . import using
from . import asm
from . import linkage
from . import attribute


@glrp.rule('declaration-seq : declaration')
@glrp.rule('declaration-seq : declaration-seq declaration')
@cxx98
def declaration_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('declaration : block-declaration')
@glrp.rule('declaration : nodeclspec-function-declaration')
@glrp.rule('declaration : function-definition')
@glrp.rule('declaration : template-declaration')
@glrp.rule('declaration : deduction-guide')
@glrp.rule('declaration : explicit-instantiation')
@glrp.rule('declaration : explicit-specialization')
@glrp.rule('declaration : export-declaration')
@glrp.rule('declaration : linkage-specification')
@glrp.rule('declaration : namespace-definition')
@glrp.rule('declaration : empty-declaration')
@glrp.rule('declaration : attribute-declaration')
@glrp.rule('declaration : module-import-declaration')
@glrp.rule('declaration : simple-declaration')
@cxx98
def declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('block-declaration : simple-declaration')
@glrp.rule('block-declaration : asm-declaration')
@glrp.rule('block-declaration : namespace-alias-definition')
@glrp.rule('block-declaration : using-declaration')
@glrp.rule('block-declaration : using-enum-declaration')
@glrp.rule('block-declaration : using-directive')
@glrp.rule('block-declaration : static_assert-declaration')
@glrp.rule('block-declaration : alias-declaration')
@glrp.rule('block-declaration : opaque-enum-declaration')
@cxx98
def block_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('nodeclspec-function-declaration : attribute-specifier-seq? declarator ";"')
@cxx98
def nodeclspec_function_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('alias-declaration : "using" "identifier" attribute-specifier-seq? "=" defining-type-id ";"')
@cxx98
def alias_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('simple-declaration : decl-specifier-seq init-declarator-list? ";"')
@glrp.rule('simple-declaration : attribute-specifier-seq decl-specifier-seq init-declarator-list ";"')
@glrp.rule(
    'simple-declaration : attribute-specifier-seq? decl-specifier-seq ref-qualifier? "[" identifier-list "]" initializer ";"'
)
@cxx98
def simple_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('static_assert-declaration : "static_­assert" "(" constant-expression ")" ";"')
@glrp.rule('static_assert-declaration : "static_­assert" "(" constant-expression "," "string-literal" ")" ";"')
@cxx98
def static_assert_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('empty-declaration : ";"')
@cxx98
def empty_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-declaration : attribute-specifier-seq ";"')
@cxx98
def attribute_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser