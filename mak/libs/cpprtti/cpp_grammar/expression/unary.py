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

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_unary_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
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


@cpp98
def p_unary_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unary-expression : postfix-expression
                         | KW_SIZEOF unary-expression
                         | KW_SIZEOF ELLIPSIS LPAREN IDENTIFIER RPAREN
                         | noexcept-expression
    """


@cpp98
def p_unary_operator(parser, p):
    # type: (CppParser, YaccProduction) -> None
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
    from ...cpp_parser import CppParser