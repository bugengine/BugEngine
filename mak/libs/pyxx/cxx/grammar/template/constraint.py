"""
constraint-expression:
    logical-or-expression
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('constraint-expression : logical-or-expression')
@cxx98
def constraint_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser