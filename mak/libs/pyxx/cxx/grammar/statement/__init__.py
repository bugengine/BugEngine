"""
statement:
    labeled-statement
    attribute-specifier-seq? expression-statement
    attribute-specifier-seq? compound-statement
    attribute-specifier-seq? selection-statement
    attribute-specifier-seq? iteration-statement
    attribute-specifier-seq? jump-statement
    declaration-statement
    attribute-specifier-seq? try-block

init-statement:
    expression-statement
    simple-declaration

condition:
    expression
    attribute-specifier-seq? decl-specifier-seq declarator brace-or-equal-initializer
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import labeled
from . import expression
from . import block
from . import selection
from . import iteration
from . import jump
from . import declaration


@glrp.rule('statement : labeled-statement')
@glrp.rule('statement : attribute-specifier-seq? expression-statement')
@glrp.rule('statement : attribute-specifier-seq? compound-statement')
@glrp.rule('statement : attribute-specifier-seq? selection-statement')
@glrp.rule('statement : attribute-specifier-seq? iteration-statement')
@glrp.rule('statement : attribute-specifier-seq? jump-statement')
@glrp.rule('statement : declaration-statement')
@glrp.rule('statement : attribute-specifier-seq? try-block')
@cxx98
def statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('init-statement : expression-statement')
@glrp.rule('init-statement : simple-declaration')
def init_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('condition : expression')
@glrp.rule('condition : attribute-specifier-seq? decl-specifier-seq declarator brace-or-equal-initializer')
def condition(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser