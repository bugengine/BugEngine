"""
base-clause:
    : base-specifier-list

base-specifier-list:
    base-specifier ...?
    base-specifier-list , base-specifier ...?

base-specifier:
    attribute-specifier-seq? class-or-decltype
    attribute-specifier-seq? virtual access-specifier? class-or-decltype
    attribute-specifier-seq? access-specifier virtual? class-or-decltype

class-or-decltype:
    nested-name-specifier? type-name
    nested-name-specifier template simple-template-id
    decltype-specifier

access-specifier:
    private
    protected
    public
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('base-clause : ":" base-specifier-list')
@cxx98
def base_clause(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('base-specifier-list : base-specifier "..."?')
@glrp.rule('base-specifier-list : base-specifier-list "," base-specifier "..."?')
@cxx98
def base_specifier_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('base-specifier : attribute-specifier-seq? class-or-decltype')
@glrp.rule('base-specifier : attribute-specifier-seq? virtual access-specifier? class-or-decltype')
@glrp.rule('base-specifier : attribute-specifier-seq? access-specifier virtual? class-or-decltype')
@cxx98
def base_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-or-decltype : nested-name-specifier? type-name')
@glrp.rule('class-or-decltype : nested-name-specifier template simple-template-id')
@glrp.rule('class-or-decltype : decltype-specifier')
@cxx98
def class_or_decltype(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('access-specifier : "private"')
@glrp.rule('access-specifier : "protected"')
@glrp.rule('access-specifier : "public"')
@cxx98
def access_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser