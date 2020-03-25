"""
assignment-expression:
      conditional-expression
      logical-or-expression assignment-operator initializer-clause     C++0x
      throw-expression
assignment-operator:
      =
      *=
      /=
      %=
      +=
      -=
      >>=
      <<=
      &=
      ^=
      |=
"""

from be_typing import TYPE_CHECKING


def p_assignment_expression(p):
    # type: (YaccProduction) -> None
    """
        assignment-expression : conditional-expression
                              | logical-or-expression assignment-operator initializer-clause
                              | throw-expression
    """


def p_assignment_operator(p):
    # type: (YaccProduction) -> None
    """
        assignment-operator : OP_EQUALS
                            | OP_TIMESEQUAL
                            | OP_DIVEQUAL
                            | OP_MODEQUAL
                            | OP_PLUSEQUAL
                            | OP_MINUSEQUAL
                            | OP_RSHIFTEQUAL
                            | OP_LSHIFTEQUAL
                            | OP_ANDEQUAL
                            | OP_XOREQUAL
                            | OP_OREQUAL
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction