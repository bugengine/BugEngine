"""
multiplicative-expression:
    pm-expression
    multiplicative-expression * pm-expression
    multiplicative-expression / pm-expression
    multiplicative-expression % pm-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('multiplicative-expression : pm-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "*" pm-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "/" pm-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "%" pm-expression')
@cxx98
def multiplicative_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser