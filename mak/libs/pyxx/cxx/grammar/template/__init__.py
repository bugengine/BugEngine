"""
template-declaration:
    template-head declaration
    template-head concept-definition

template-head:
    template < template-parameter-list > requires-clause?

template-parameter-list:
    template-parameter
    template-parameter-list , template-parameter

requires-clause:
    requires constraint-logical-or-expression

constraint-logical-or-expression:
    constraint-logical-and-expression
    constraint-logical-or-expression || constraint-logical-and-expression

constraint-logical-and-expression:
    primary-expression
    constraint-logical-and-expression && primary-expression
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import parameter
from . import name
from . import explicit
from . import concept
from . import constraint
from . import guide


@glrp.rule('template-declaration : template-head declaration')
@glrp.rule('template-declaration : template-head concept-definition')
@cxx98
def template_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-head : "template" "<" template-parameter-list ">" requires-clause?')
@cxx98
def template_head(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-parameter-list : template-parameter')
@glrp.rule('template-parameter-list : template-parameter-list "," template-parameter')
@cxx98
def template_parameter_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('requires-clause : "requires" constraint-logical-or-expression')
@cxx98
def requires_clause(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('constraint-logical-or-expression : constraint-logical-and-expression')
@glrp.rule('constraint-logical-or-expression : constraint-logical-or-expression "||" constraint-logical-and-expression')
@cxx98
def constraint_logical_or_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('constraint-logical-and-expression : primary-expression')
@glrp.rule('constraint-logical-and-expression : constraint-logical-and-expression "&&" primary-expression')
@cxx98
def constraint_logical_and_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser