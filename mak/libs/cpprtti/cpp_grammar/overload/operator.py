"""
operator-function-id: 	See C++ Standard Core Language Issue n. 189
      operator overloadable-operator
      operator overloadable-operator < template-argument-listopt >
overloadable-operator: 	See C++ Standard Core Language Issue n. 189
      new
      delete
      new [ ]
      delete [ ]
      +
      -
      *
      /
      %
      ^
      &
      |
      ~
      !
      =
      <
      >
      +=
      -=
      *=
      /=
      %=
      ^=
      &=
      |=
      <<
      >>
      >>=
      <<=
      ==
      !=
      <=
      >=
      &&
      ||
      ++
      --
      ,
      ->*
      ->
      ()
      []
"""

from be_typing import TYPE_CHECKING


def p_operator_function_id(p):
    # type: (YaccProduction) -> None
    """
        operator-function-id : KW_OPERATOR overloadable-operator
    """
    # removed template args.


def p_overloadable_operator(p):
    # type: (YaccProduction) -> None
    """
        overloadable-operator : KW_NEW
                              | KW_DELETE
                              | KW_NEW LBRACKET RBRACKET
                              | KW_DELETE LBRACKET RBRACKET
                              | OP_PLUS
                              | OP_MINUS
                              | OP_TIMES
                              | OP_DIVIDE
                              | OP_MOD
                              | OP_XOR
                              | OP_AND
                              | OP_OR
                              | OP_NOT
                              | OP_LNOT
                              | OP_EQUALS
                              | OP_LT
                              | OP_GT
                              | OP_PLUSEQUAL
                              | OP_MINUSEQUAL
                              | OP_TIMESEQUAL
                              | OP_DIVEQUAL
                              | OP_MODEQUAL
                              | OP_XOREQUAL
                              | OP_ANDEQUAL
                              | OP_OREQUAL
                              | OP_LSHIFT
                              | OP_RSHIFT
                              | OP_LSHIFTEQUAL
                              | OP_RSHIFTEQUAL
                              | OP_EQ
                              | OP_NE
                              | OP_LE
                              | OP_GE
                              | OP_LAND
                              | OP_LOR
                              | OP_PLUSPLUS
                              | OP_MINUSMINUS
                              | COMMA
                              | OP_ARROWMEMBER
                              | OP_ARROW
                              | LPAREN RPAREN
                              | LBRACKET RBRACKET
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction