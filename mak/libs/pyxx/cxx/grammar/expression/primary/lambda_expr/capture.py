"""
lambda-capture:
    capture-default
    capture-list
    capture-default , capture-list

capture-default:
    &
    =

capture-list:
    capture
    capture-list , capture

capture:
    simple-capture
    init-capture

simple-capture:
    identifier ...?
    & identifier ...?
    this
    * this

init-capture:
    ...?  identifier initializer
    & ...? identifier initializer
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('lambda-capture : capture-default')
@glrp.rule('lambda-capture : capture-list')
@glrp.rule('lambda-capture : capture-default "," capture-list')
@cxx98
def lambda_capture(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('capture-default : "&"')
@glrp.rule('capture-default : "="')
@cxx98
def capture_default(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('capture-list : capture')
@glrp.rule('capture-list : capture-list "," capture')
@cxx98
def capture_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('capture : simple-capture')
@glrp.rule('capture : init-capture')
@cxx98
def capture(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('simple-capture : "identifier" [split] "..."?')
@glrp.rule('simple-capture : "&" "identifier" "..."?')
@glrp.rule('simple-capture :"this"')
@glrp.rule('simple-capture :"*" "this"')
@cxx98
def simple_capture(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('init-capture : "..."? "identifier" initializer')
@glrp.rule('init-capture : "&" "..."? "identifier" initializer')
@cxx98
def init_capture(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser