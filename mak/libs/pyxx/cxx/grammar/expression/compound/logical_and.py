"""
logical-and-expression:
    inclusive-or-expression
    logical-and-expression && inclusive-or-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('logical-and-expression : inclusive-or-expression')
@glrp.rule('logical-and-expression : logical-and-expression "&&" inclusive-or-expression')
@cxx98
def logical_and_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser