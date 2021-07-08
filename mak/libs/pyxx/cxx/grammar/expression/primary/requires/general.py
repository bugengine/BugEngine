"""
requires-expression:
    requires requirement-parameter-list? requirement-body

requirement-parameter-list:
    ( parameter-declaration-clause )

requirement-body:
    { requirement-seq }

requirement-seq:
    requirement
    requirement-seq requirement

requirement:
    simple-requirement
    type-requirement
    compound-requirement
    nested-requirement
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('requires-expression : requires requirement-parameter-list? requirement-body')
@cxx98
def requires_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('requirement-parameter-list : "(" parameter-declaration-clause ")"')
@cxx98
def requires_parameter_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('requirement-body : "{" requirement-seq "}"')
@cxx98
def requirement_body(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('requirement-seq : requirement')
@glrp.rule('requirement-seq : requirement-seq requirement')
@cxx98
def requirement_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('requirement : simple-requirement')
@glrp.rule('requirement : type-requirement')
@glrp.rule('requirement : compound-requirement')
@glrp.rule('requirement : nested-requirement')
@cxx98
def requirement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser
