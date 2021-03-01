"""
postfix-expression:
      primary-expression
      postfix-expression [ expression ]
      postfix-expression [ braced-init-listopt ]     C++0x
      postfix-expression ( expression-listopt )
      simple-type-specifier ( expression-listopt )
      typename-specifier ( expression-listopt )
      simple-type-specifier braced-init-list     C++0x
      typename-specifier braced-init-list     C++0x
      postfix-expression . templateopt id-expression
      postfix-expression -> templateopt id-expression
      postfix-expression . pseudo-destructor-name
      postfix-expression -> pseudo-destructor-name
      postfix-expression ++
      postfix-expression --
      dynamic_cast < type-id > ( expression )
      static_cast < type-id > ( expression )
      reinterpret_cast < type-id > ( expression )
      const_cast < type-id > ( expression )
      typeid ( expression )
      typeid ( type-id )
expression-list:
      initializer-list
pseudo-destructor-name:
      ::opt nested-name-specifieropt type-name :: ~ type-name
      ::opt nested-name-specifier template simple-template-id :: ~ type-name     C++0x
      ::opt nested-name-specifieropt ~ type-name
      ~ decltype-specifier     C++0x
"""

from ...cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@cpp98
def p_postfix_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        postfix-expression : primary-expression
                           | postfix-expression RBRACKET expression RBRACKET
                           | postfix-expression LPAREN expression-list? RPAREN
                           | simple-type-specifier LPAREN expression-list? RPAREN
                           | typename-specifier LPAREN expression-list? RPAREN
                           | postfix-expression PERIOD KW_TEMPLATE? id-expression
                           | postfix-expression OP_ARROW KW_TEMPLATE? id-expression
                           | postfix-expression OP_PLUSPLUS
                           | postfix-expression OP_MINUSMINUS
                           | KW_DYNAMIC_CAST OP_LT type-id OP_GT LPAREN expression RPAREN
                           | KW_STATIC_CAST OP_LT type-id OP_GT LPAREN expression RPAREN
                           | KW_REINTERPRET_CAST OP_LT type-id OP_GT LPAREN expression RPAREN
                           | KW_CONST_CAST OP_LT type-id OP_GT LPAREN expression RPAREN
                           | KW_TYPEID LPAREN expression RPAREN
                           | KW_TYPEID LPAREN type-id RPAREN
    """


@cpp11
def p_postfix_expression_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        postfix-expression : postfix-expression RBRACKET braced-init-list? RBRACKET
                           | simple-type-specifier braced-init-list
                           | typename-specifier braced-init-list
    """


@disabled
def p_postfix_expression_disabled(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        postfix-expression : postfix-expression PERIOD pseudo-destructor-name
                           | postfix-expression OP_ARROW pseudo-destructor-name
    """
    # disabled, as KW_TEMPLATE? id-expression covers pseudo-destructor-name
    # type-id covers constructors/function calls/etc


@cpp98
def p_expression_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        expression-list : initializer-list
    """


@disabled
def p_pseudo_destructor_name(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        pseudo-destructor-name : OP_SCOPE? nested-name-specifier? type-name OP_SCOPE OP_NOT type-name
                               | OP_SCOPE? nested-name-specifier KW_TEMPLATE simple-template-id OP_SCOPE OP_NOT type-name
                               | OP_SCOPE? nested-name-specifier? OP_NOT type-name
                               | OP_NOT decltype-specifier
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser