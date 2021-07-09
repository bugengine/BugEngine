"""
parameter-declaration-clause:
    parameter-declaration-list? ...?
    parameter-declaration-list , ...

parameter-declaration-list:
    parameter-declaration
    parameter-declaration-list , parameter-declaration

parameter-declaration:
    attribute-specifier-seq? decl-specifier-seq declarator
    attribute-specifier-seq? decl-specifier-seq declarator = initializer-clause
    attribute-specifier-seq? decl-specifier-seq abstract-declarator?
    attribute-specifier-seq? decl-specifier-seq abstract-declarator? = initializer-clause
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('parameter-declaration-clause : parameter-declaration-list? "..."?')
@glrp.rule('parameter-declaration-clause : parameter-declaration-list "," "..."')
@cxx98
def parameter_declaration_clause(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('parameter-declaration-list : parameter-declaration')
@glrp.rule('parameter-declaration-list : parameter-declaration-list "," parameter-declaration')
@cxx98
def parameter_declaration_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator')
@glrp.rule('parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator "=" initializer-clause')
@glrp.rule('parameter-declaration : attribute-specifier-seq? decl-specifier-seq abstract-declarator?')
@glrp.rule(
    'parameter-declaration : attribute-specifier-seq? decl-specifier-seq abstract-declarator? "=" initializer-clause'
)
@cxx98
def parameter_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser