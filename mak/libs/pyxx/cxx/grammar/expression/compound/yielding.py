"""
yield-expression:
    co_yield assignment-expression
    co_yield braced-init-list
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('yield-expression : "co_yield" assignment-expression')
@glrp.rule('yield-expression : "co_yield" braced-init-list')
@cxx98
def yield_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser