"""
relational-expression:
    compare-expression
    relational-expression < compare-expression
    relational-expression > compare-expression
    relational-expression <= compare-expression
    relational-expression >= compare-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('relational-expression : compare-expression')
@glrp.rule('relational-expression : relational-expression "<" compare-expression')
@glrp.rule('relational-expression : relational-expression ">" compare-expression')
@glrp.rule('relational-expression : relational-expression "<=" compare-expression')
@glrp.rule('relational-expression : relational-expression ">=" compare-expression')
@cxx98
def relational_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser