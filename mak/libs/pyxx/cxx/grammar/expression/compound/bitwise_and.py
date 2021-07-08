"""
and-expression:
    equality-expression
    and-expression & equality-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('and-expression : equality-expression')
@glrp.rule('and-expression : and-expression "&" equality-expression')
@cxx98
def and_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser