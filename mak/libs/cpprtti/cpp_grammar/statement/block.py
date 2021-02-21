"""
compound-statement:
      { statement-seqopt }
statement-seq:
      statement
      statement-seq statement
"""

from be_typing import TYPE_CHECKING


def p_compound_statement(p):
    # type: (YaccProduction) -> None
    """
        compound-statement : LBRACE statement-seq? RBRACE
    """


def p_statement_seq(p):
    # type: (YaccProduction) -> None
    """
        statement-seq : statement
                      | statement-seq statement
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction