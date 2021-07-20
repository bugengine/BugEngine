"""
equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('equality-expression[prec:right,0] : relational-expression')
@glrp.rule('equality-expression : equality-expression "==" relational-expression')
@glrp.rule('equality-expression : equality-expression "!=" relational-expression')
@cxx98
def equality_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser