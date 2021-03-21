"""
compound-statement:
      { statement-seqopt }
statement-seq:
      statement
      statement-seq statement
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("compound-statement : '{' statement-seq? '}'")
@cxx98
def p_compound_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("statement-seq : statement | statement-seq statement")
@cxx98
def p_statement_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser