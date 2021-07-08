"""
exclusive-or-expression:
    and-expression
    exclusive-or-expression ^ and-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('exclusive-or-expression : and-expression')
@glrp.rule('exclusive-or-expression : exclusive-or-expression "^" and-expression')
@cxx98
def exclusive_or_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser