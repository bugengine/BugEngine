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
def p_id_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        id-expression : unqualified-id
                      | qualified-id
    """


@cpp98
def p_unqualified_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-id : IDENTIFIER template-spec?
                       | operator-function-id template-spec?
                       | conversion-function-id
                       | OP_NOT IDENTIFIER
    """


@cpp11
def p_unqualified_id_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unqualified-id : literal-operator-id template-spec?
                       | OP_NOT decltype-specifier
    """


@cpp98
def p_qualified_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-id : nested-name-specifier KW_TEMPLATE? unqualified-id
                     | OP_SCOPE nested-name-specifier KW_TEMPLATE? unqualified-id   %prec SCOPE_REDUCTION
                     | OP_SCOPE IDENTIFIER template-spec?                           %prec SCOPE_REDUCTION
                     | OP_SCOPE operator-function-id template-spec?                 %prec SCOPE_REDUCTION
    """


@cpp11
def p_qualified_id_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-id : OP_SCOPE literal-operator-id template-spec?                  %prec SCOPE_REDUCTION
    """


@cpp98
def p_nested_name_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        nested-name-specifier : IDENTIFIER template-spec? OP_SCOPE
                              | nested-name-specifier KW_TEMPLATE IDENTIFIER template-spec? OP_SCOPE
                              | nested-name-specifier IDENTIFIER template-spec? OP_SCOPE
    """


@cpp11
def p_nested_name_specifier_decltype(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        nested-name-specifier : decltype-specifier OP_SCOPE
    """


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


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser