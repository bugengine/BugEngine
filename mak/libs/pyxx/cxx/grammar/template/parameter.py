"""
template-parameter:
    type-parameter
    parameter-declaration

type-parameter:
    type-parameter-key ...? identifier?
    type-parameter-key identifier? = type-id
    type-constraint ...? identifier?
    type-constraint identifier? = type-id
    template-head type-parameter-key ...? identifier?
    template-head type-parameter-key identifier? = id-expression

type-parameter-key:
    class
    typename

type-constraint:
    nested-name-specifier? concept-name
    nested-name-specifier? concept-name < template-argument-list? >
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('template-parameter : type-parameter')
@glrp.rule('template-parameter : parameter-declaration')
@cxx98
def template_parameter(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('type-parameter : type-parameter-key "..."? "identifier"?')
@glrp.rule('type-parameter : type-parameter-key "identifier"? "=" type-id')
@glrp.rule('type-parameter : type-constraint "..."? "identifier"?')
@glrp.rule('type-parameter : type-constraint "identifier"? "=" type-id')
@glrp.rule('type-parameter : template-head type-parameter-key "..."? "identifier"?')
@glrp.rule('type-parameter : template-head type-parameter-key "identifier"? "=" id-expression')
@cxx98
def type_parameter(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('type-parameter-key : "class"')
@glrp.rule('type-parameter-key : "typename"')
@cxx98
def type_parameter_key(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('type-constraint : nested-name-specifier? concept-name')
@glrp.rule('type-constraint : nested-name-specifier? concept-name "<" template-argument-list? ">"')
@cxx98
def type_constraint(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser