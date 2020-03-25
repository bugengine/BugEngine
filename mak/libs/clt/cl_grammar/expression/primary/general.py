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
"""

from be_typing import TYPE_CHECKING


def p_primary_expression(p):
    # type: (YaccProduction) -> None
    """
        primary-expression : literal
                           | KW_THIS
                           | LPAREN expression RPAREN
                           | id-expression
                           | lambda-expression
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
        unqualified-id : identifier
                       | operator-function-id
                       | conversion-function-id
                       | literal-operator-id
                       | OP_NOT type-name
                       | OP_NOT decltype-specifier
                       | template-id
    """
    # does not accept simple-template-id


def p_qualified_id(p):
    # type: (YaccProduction) -> None
    """
        qualified-id : OP_SCOPE nested-name-specifier KW_TEMPLATE unqualified-id
                     | nested-name-specifier KW_TEMPLATE unqualified-id
                     | OP_SCOPE nested-name-specifier unqualified-id
                     | nested-name-specifier unqualified-id
                     | OP_SCOPE identifier
                     | OP_SCOPE operator-function-id
                     | OP_SCOPE literal-operator-id
                     | OP_SCOPE template-id
                     | OP_SCOPE simple-template-id
    """


def p_nested_name_specifier(p):
    # type: (YaccProduction) -> None
    """
        nested-name-specifier : type-name OP_SCOPE
                              | namespace-name OP_SCOPE
                              | decltype-specifier OP_SCOPE
                              | nested-name-specifier identifier OP_SCOPE
                              | nested-name-specifier KW_TEMPLATE simple-template-id OP_SCOPE
                              | nested-name-specifier simple-template-id OP_SCOPE
    """


def p_nested_name_specifier_opt(p):
    # type: (YaccProduction) -> None
    """
        nested-name-specifier-opt : nested-name-specifier
    """
    # TODO: empty


def p_scope_opt(p):
    # type: (YaccProduction) -> None
    """
        scope-opt : OP_SCOPE
    """
    # TODO: empty


def p_indentifier(p):
    # type: (YaccProduction) -> None
    """
        identifier : IDENTIFIER
    """


def p_indentifier_opt(p):
    # type: (YaccProduction) -> None
    """
        identifier-opt : IDENTIFIER
    """
    # TODO: empty


def p_literal(p):
    # type: (YaccProduction) -> None
    """
        literal : STRING_LITERAL
                | INTEGER_LITERAL
                | FLOATING_LITERAL
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction