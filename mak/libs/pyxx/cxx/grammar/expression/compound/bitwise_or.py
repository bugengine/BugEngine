"""
inclusive-or-expression:
    exclusive-or-expression
    inclusive-or-expression | exclusive-or-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('inclusive-or-expression : exclusive-or-expression')
@glrp.rule('inclusive-or-expression : inclusive-or-expression "|" exclusive-or-expression')
@cxx98
def inclusive_or_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser