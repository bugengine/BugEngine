"""
init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator

init-declarator:
    declarator initializer?
    declarator requires-clause
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING
from . import declarator
from . import name
from . import function


@glrp.rule('init-declarator-list : init-declarator')
@glrp.rule('init-declarator-list : init-declarator-list "," init-declarator')
@cxx98
def init_declarator_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('init-declarator : declarator initializer?')
@glrp.rule('init-declarator : declarator requires-clause')
@cxx98
def init_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser