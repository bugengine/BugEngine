"""
conditional-expression:
    logical-or-expression
    logical-or-expression ? expression : assignment-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('conditional-expression : logical-or-expression')
@glrp.rule('conditional-expression : logical-or-expression "?" expression ":" assignment-expression')
@cxx98
def conditional_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser