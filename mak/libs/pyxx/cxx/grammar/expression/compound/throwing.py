"""
throw-expression:
    throw assignment-expression?
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('throw-expression : "throw" assignment-expression?')
@cxx98
def throw_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser