"""
simple-template-id:
    template-name < template-argument-list? >

template-id:
    simple-template-id
    operator-function-id < template-argument-list? >
    literal-operator-id < template-argument-list? >

template-name:
    identifier

template-argument-list:
    template-argument ...?
    template-argument-list , template-argument ...?

template-argument:
    constant-expression
    type-id
    id-expression

typename-specifier:
    typename nested-name-specifier identifier
    typename nested-name-specifier template? simple-template-id
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('simple-template-id : template-name "<" template-argument-list? ">"')
@cxx98
def simple_template_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-id : simple-template-id')
@glrp.rule('template-id : operator-function-id "<" template-argument-list? ">"')
@glrp.rule('template-id : literal-operator-id "<" template-argument-list? ">"')
@cxx98
def template_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-name : "identifier"')
@cxx98
def template_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-argument-list : template-argument "..."?')
@glrp.rule('template-argument-list : template-argument-list "," template-argument "..."?')
@cxx98
def template_argument_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('template-argument : constant-expression')
@glrp.rule('template-argument : type-id')
@glrp.rule('template-argument : id-expression')
@cxx98
def template_argument(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('typename-specifier : typename nested-name-specifier "identifier"')
@glrp.rule('typename-specifier : typename nested-name-specifier "template"? simple-template-id')
@cxx98
def typename_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser