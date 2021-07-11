"""
primary-expression:
    literal
    this
    ( expression )
    id-expression
    lambda-expression
    fold-expression
    requires-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING
from . import id
from . import lambda_expr
from . import fold
from . import requires


@glrp.rule('primary-expression : "integer-literal"')
@glrp.rule('primary-expression : "character-literal"')
@glrp.rule('primary-expression : "floating-literal"')
@glrp.rule('primary-expression : "string-literal"')
@glrp.rule('primary-expression : "user-defined-integer-literal"')
@glrp.rule('primary-expression : "user-defined-character-literal"')
@glrp.rule('primary-expression : "user-defined-floating-literal"')
@glrp.rule('primary-expression : "user-defined-string-literal"')
@glrp.rule('primary-expression : "this"')
@glrp.rule('primary-expression : "(" expression ")"')
@glrp.rule('primary-expression : id-expression')
@glrp.rule('primary-expression : lambda-expression')
@glrp.rule('primary-expression : fold-expression')
@glrp.rule('primary-expression : requires-expression')
@cxx98
def primary_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser