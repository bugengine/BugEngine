"""
shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('shift-expression : additive-expression')
@glrp.rule('shift-expression : shift-expression "<<" additive-expression')
@glrp.rule('shift-expression : shift-expression ">>" additive-expression')
@cxx98
def shift_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser