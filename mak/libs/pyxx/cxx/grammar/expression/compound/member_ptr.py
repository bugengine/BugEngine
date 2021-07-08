"""
pm-expression:
    cast-expression
    pm-expression .* cast-expression
    pm-expression ->* cast-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('pm-expression : cast-expression')
@glrp.rule('pm-expression : pm-expression ".*" cast-expression')
@glrp.rule('pm-expression : pm-expression "->*" cast-expression')
@cxx98
def pm_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser