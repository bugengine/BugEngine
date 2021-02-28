"""
compound-statement:
      { statement-seqopt }
statement-seq:
      statement
      statement-seq statement
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_compound_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        compound-statement : LBRACE statement-seq? RBRACE
    """


@cpp98
def p_statement_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        statement-seq : statement
                      | statement-seq statement
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser