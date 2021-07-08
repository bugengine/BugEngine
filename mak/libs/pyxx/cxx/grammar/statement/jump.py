"""
jump-statement:
    break ;
    continue ;
    return expr-or-braced-init-list? ;
    coroutine-return-statement
    goto identifier ;

coroutine-return-statement:
    co_return expr-or-braced-init-list? ;
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('jump-statement : "break" ";"')
@glrp.rule('jump-statement : "continue" ";"')
@glrp.rule('jump-statement : "return" expr-or-braced-init-list? ";"')
@glrp.rule('jump-statement : coroutine-return-statement')
@glrp.rule('jump-statement : "goto" "identifier" ";"')
@cxx98
def jump_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('coroutine-return-statement : "co_return" expr-or-braced-init-list? ";"')
@cxx98
def coroutine_return_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser