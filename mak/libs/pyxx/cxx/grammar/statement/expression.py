"""
expression-statement:
    expression? ;
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('expression-statement : expression? ";"')
@cxx98
def expression_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser