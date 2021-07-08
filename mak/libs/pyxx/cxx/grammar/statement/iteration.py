"""
iteration-statement:
    while ( condition ) statement
    do statement while ( expression ) ;
    for ( init-statement condition? ; expression? ) statement
    for ( init-statement? for-range-declaration : for-range-initializer ) statement

for-range-declaration:
    attribute-specifier-seq? decl-specifier-seq declarator
    attribute-specifier-seq? decl-specifier-seq ref-qualifier? [ identifier-list ]

for-range-initializer:
    expr-or-braced-init-list
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('iteration-statement : "while" "(" condition ")" statement')
@glrp.rule('iteration-statement : "do" statement "while" "(" expression ")" ";"')
@glrp.rule('iteration-statement : "for" "(" init-statement condition? ";" expression? ")" statement')
@glrp.rule(
    'iteration-statement : "for" "(" init-statement? for-range-declaration ":" for-range-initializer ")" statement'
)
@cxx98
def iteration_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('for-range-declaration : attribute-specifier-seq? decl-specifier-seq declarator')
@glrp.rule('for-range-declaration : attribute-specifier-seq? decl-specifier-seq ref-qualifier? "[" identifier-list "]"')
@cxx98
def for_range_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('for-range-initializer : expr-or-braced-init-list')
@cxx98
def for_range_initializer(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser