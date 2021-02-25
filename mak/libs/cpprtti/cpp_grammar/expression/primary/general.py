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
        unqualified-id : IDENTIFIER template-spec?
                       | operator-function-id template-spec?
                       | conversion-function-id
                       | literal-operator-id template-spec?
                       | OP_NOT IDENTIFIER
                       | OP_NOT decltype-specifier
    """


def p_qualified_id(p):
    # type: (YaccProduction) -> None
    """
        qualified-id : nested-name-specifier KW_TEMPLATE unqualified-id
                     | nested-name-specifier unqualified-id
                     | OP_SCOPE nested-name-specifier KW_TEMPLATE unqualified-id
                     | OP_SCOPE nested-name-specifier unqualified-id
                     | OP_SCOPE KW_TEMPLATE IDENTIFIER                              %prec SCOPE_REDUCTION
                     | OP_SCOPE IDENTIFIER template-spec?                           %prec SCOPE_REDUCTION
                     | OP_SCOPE operator-function-id template-spec?                 %prec SCOPE_REDUCTION
                     | OP_SCOPE literal-operator-id template-spec?                  %prec SCOPE_REDUCTION
    """


def p_nested_name_specifier(p):
    # type: (YaccProduction) -> None
    """
        nested-name-specifier : IDENTIFIER template-spec? OP_SCOPE
                              | decltype-specifier OP_SCOPE
                              | nested-name-specifier KW_TEMPLATE IDENTIFIER template-spec? OP_SCOPE
                              | nested-name-specifier IDENTIFIER template-spec? OP_SCOPE
    """


def p_literal(p):
    # type: (YaccProduction) -> None
    """
        literal : STRING_LITERAL
                | INTEGER_LITERAL
                | FLOATING_LITERAL
    """


def p_template_spec(p):
    # type: (YaccProduction) -> None
    """
        template-spec : LANGLE template-argument-list? RANGLE
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction