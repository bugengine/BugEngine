"""
compound-statement:
    { statement-seq? }
statement-seq:
    statement
    statement-seq statement
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


#@glrp.rule('compound-statement : "{" statement-seq? "}"')
@glrp.rule('compound-statement[split] : "{" "}"')
@glrp.rule('compound-statement : "{" statement-seq "}"')
@cxx98
def compound_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('statement-seq : statement')
@glrp.rule('statement-seq : statement-seq statement')
@cxx98
def statement_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser