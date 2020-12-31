"""
equality-expression:
      relational-expression
      equality-expression == relational-expression
      equality-expression != relational-expression
"""

from be_typing import TYPE_CHECKING


def p_equality_expression(p):
    # type: (YaccProduction) -> None
    """
        equality-expression : relational-expression
                            | equality-expression OP_EQ relational-expression
                            | equality-expression OP_NE relational-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction