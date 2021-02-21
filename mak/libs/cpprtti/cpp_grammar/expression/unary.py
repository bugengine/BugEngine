"""
unary-expression:
      postfix-expression
      ++ cast-expression
      -- cast-expression
      unary-operator cast-expression
      sizeof unary-expression
      sizeof ( type-id )
      sizeof ... ( identifier )     C++0x
      alignof ( type-id )     C++0x
      noexcept-expression     C++0x
      new-expression
      delete-expression
unary-operator:
      *
      &
      +
      -
      !
      ~
"""

from be_typing import TYPE_CHECKING


def p_unary_expression(p):
    # type: (YaccProduction) -> None
    """
        unary-expression : postfix-expression
                         | OP_PLUSPLUS cast-expression
                         | OP_MINUSMINUS cast-expression
                         | unary-operator cast-expression
                         | KW_SIZEOF unary-expression
                         | KW_SIZEOF LPAREN type-id RPAREN
                         | KW_SIZEOF ELLIPSIS LPAREN IDENTIFIER RPAREN
                         | KW_ALIGNOF LPAREN type-id RPAREN
                         | noexcept-expression
                         | new-expression
                         | delete-expression
    """


def p_unary_operator(p):
    # type: (YaccProduction) -> None
    """
        unary-operator : OP_TIMES
                       | OP_AND
                       | OP_PLUS
                       | OP_MINUS
                       | OP_LNOT
                       | OP_NOT
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction