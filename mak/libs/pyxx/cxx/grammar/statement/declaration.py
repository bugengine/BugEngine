"""
declaration-statement:
    block-declaration
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('declaration-statement : block-declaration')
@cxx98
def declaration_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser