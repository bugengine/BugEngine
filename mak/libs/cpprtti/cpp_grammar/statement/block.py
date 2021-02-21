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
        compound-statement : LBRACE statement-seq-opt RBRACE
    """


def p_statement_seq(p):
    # type: (YaccProduction) -> None
    """
        statement-seq : statement
                      | statement-seq statement
    """


def p_statement_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        statement-seq-opt : statement-seq
                          | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction