"""
selection-statement:
    if constexpr? ( init-statement? condition ) statement
    if constexpr? ( init-statement? condition ) statement else statement
    if !? consteval compound-statement
    if !? consteval compound-statement else statement
    switch ( init-statement? condition ) statement
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('selection-statement : "if" "constexpr"? "(" init-statement? condition ")" statement')
@glrp.rule('selection-statement : "if" "constexpr"? "(" init-statement? condition ")" statement "else" statement')
@glrp.rule('selection-statement : "if" "!"? "consteval" compound-statement')
@glrp.rule('selection-statement : "if" "!"? "consteval" compound-statement "else" statement')
@glrp.rule('selection-statement : "switch" "(" init-statement? condition ")" statement')
@cxx98
def selection_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser