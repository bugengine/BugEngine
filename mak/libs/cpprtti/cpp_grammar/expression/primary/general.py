"""
primary-expression: 	 
      literal
      this
      ( expression )
      id-expression
      lambda-expression     C++0x
id-expression: 	 
      unqualified-id
      qualified-id
unqualified-id: 	 
      identifier
      operator-function-id
      conversion-function-id
      literal-operator-id     C++0x
      ~ class-name
      ~ decltype-specifier     C++0x
      template-id
qualified-id: 	 
      ::opt nested-name-specifier templateopt unqualified-id
      :: identifier
      :: operator-function-id
      :: literal-operator-id     C++0x
      :: template-id
nested-name-specifier: 	 
      type-name ::
      namespace-name ::
      decltype-specifier ::     C++0x
      nested-name-specifier identifier ::
      nested-name-specifier templateopt simple-template-id ::

CHANGES:
========
    moved lambda-expression into expression (comma.py) to avoid constant-expression to match lambdas
"""

from be_typing import TYPE_CHECKING


def p_primary_expression(p):
    # type: (YaccProduction) -> None
    """
        primary-expression : literal
                           | KW_THIS
                           | LPAREN expression RPAREN
                           | id-expression
    """


def p_id_expression(self):
    # type: (YaccProduction) -> None
    """
        id-expression : unqualified-id
                      | qualified-id
    """


def p_unqualified_id(p):
    # type: (YaccProduction) -> None
    """
        unqualified-id : IDENTIFIER
                       | operator-function-id
                       | conversion-function-id
                       | literal-operator-id
                       | OP_NOT IDENTIFIER
                       | OP_NOT decltype-specifier
                       | template-id
    """


def p_qualified_id(p):
    # type: (YaccProduction) -> None
    """
        qualified-id : OP_SCOPE? nested-name-specifier KW_TEMPLATE? unqualified-id
                     | OP_SCOPE IDENTIFIER
                     | OP_SCOPE operator-function-id
                     | OP_SCOPE literal-operator-id
                     | OP_SCOPE template-id
    """


def p_nested_name_specifier(p):
    # type: (YaccProduction) -> None
    """
        nested-name-specifier : type-name OP_SCOPE
                              | decltype-specifier OP_SCOPE
                              | nested-name-specifier IDENTIFIER OP_SCOPE
                              | nested-name-specifier KW_TEMPLATE? simple-template-id OP_SCOPE
    """


def p_literal(p):
    # type: (YaccProduction) -> None
    """
        literal : STRING_LITERAL
                | INTEGER_LITERAL
                | FLOATING_LITERAL
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction