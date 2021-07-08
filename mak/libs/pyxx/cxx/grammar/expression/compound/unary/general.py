"""
unary-expression:
    postfix-expression
    unary-operator cast-expression
    ++ cast-expression
    -- cast-expression
    await-expression
    sizeof unary-expression
    sizeof ( type-id )
    sizeof ... ( identifier )
    alignof ( type-id )
    noexcept-expression
    new-expression
    delete-expression

unary-operator: one of
    *  &  +  -  !  ~
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('unary-expression : postfix-expression')
@glrp.rule('unary-expression : unary-operator cast-expression')
@glrp.rule('unary-expression : "++" cast-expression')
@glrp.rule('unary-expression : "--" cast-expression')
@glrp.rule('unary-expression : await-expression')
@glrp.rule('unary-expression : "sizeof" unary-expression')
@glrp.rule('unary-expression : "sizeof" "(" type-id ")"')
@glrp.rule('unary-expression : "sizeof" "..." "(" identifier "")"')
@glrp.rule('unary-expression : "alignof" "(" type-id ")"')
@glrp.rule('unary-expression : noexcept-expression')
@glrp.rule('unary-expression : new-expression')
@glrp.rule('unary-expression : delete-expression')
@cxx98
def unary_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('unary-operator : "*" | "&" | "+" | "-" | "!" | "~"')
@cxx98
def unary_operator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser