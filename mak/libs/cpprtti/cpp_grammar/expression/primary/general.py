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

from ....cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@cpp98
def p_primary_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        primary-expression : literal
                           | KW_THIS
                           | LPAREN expression RPAREN
                           | id-expression
    """


@cpp11
def p_primary_expression_lambda(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        primary-expression : lambda-expression
    """


@cpp98
def p_id_type(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        id-type : unqualified-type-id
                | qualified-type-id
    """
    p[0] = p[1]


@cpp98
def p_unqualified_type_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-type-id : IDENTIFIER template-spec?                                         %prec SCOPE_REDUCTION
    """
    p[0] = p[1]


@cpp98
def p_qualified_type_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-type-id : nested-name-specifier KW_TEMPLATE? unqualified-type-id
                          | OP_SCOPE nested-name-specifier KW_TEMPLATE? unqualified-type-id
                          | OP_SCOPE IDENTIFIER template-spec?                                  %prec SCOPE_REDUCTION
    """


@cpp98
def p_id_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        id-expression : unqualified-expression-id
                      | qualified-expression-id
    """
    p[0] = p[1]


@cpp98
def p_unqualified_expression_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-expression-id : operator-function-id template-spec?
                                  | conversion-function-id
                                  | OP_NOT IDENTIFIER
    """


@cpp11
def p_unqualified_expression_id_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-expression-id : literal-operator-id template-spec?
                                  | OP_NOT decltype-specifier
    """


@cpp11
def p_unqualified_expression_id_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-expression-id : literal-operator-id template-spec?
    """


@cpp98
def p_qualified_expression_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-expression-id : nested-name-specifier KW_TEMPLATE? unqualified-expression-id
                                | OP_SCOPE nested-name-specifier KW_TEMPLATE? unqualified-expression-id
                                | OP_SCOPE operator-function-id template-spec?
    """


@cpp11
def p_qualified_expression_id_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-expression-id : OP_SCOPE literal-operator-id template-spec?
    """


@cpp98
def p_nested_name_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        nested-name-specifier : IDENTIFIER template-spec? OP_SCOPE
                              | nested-name-specifier KW_TEMPLATE? IDENTIFIER template-spec? OP_SCOPE
    """


@disabled
def p_nested_name_specifier_decltype(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        nested-name-specifier : decltype-specifier OP_SCOPE
    """
    # TODO: cpp11


@cpp98
def p_literal(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        literal : STRING_LITERAL
                | INTEGER_LITERAL
                | FLOATING_LITERAL
    """


@cpp98
def p_template_spec(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-spec : LANGLE template-argument-list? RANGLE
    """


@cpp98
def p_template_spec(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-spec : LANGLE RANGLE
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser