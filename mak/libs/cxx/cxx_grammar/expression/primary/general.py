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

from ....cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("primary-expression : literal | 'this' | '(' expression ')' | id-expression")
@cxx98
def p_primary_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("primary-expression : lambda-expression")
@cxx11
def p_primary_expression_lambda(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("id-type : unqualified-type-id | qualified-type-id")
@cxx98
def p_id_type(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("unqualified-type-id [prec:left,0] : 'identifier' template-spec?")
@cxx98
def p_unqualified_type_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("qualified-type-id : nested-name-specifier 'template'? unqualified-type-id")
@glrp.rule("qualified-type-id : [prec:left,1] '::' nested-name-specifier 'template'? unqualified-type-id")
@glrp.rule("qualified-type-id : [prec:left,1] '::' 'identifier' template-spec?")
@cxx98
def p_qualified_type_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("id-expression : unqualified-expression-id | qualified-expression-id")
@cxx98
def p_id_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("unqualified-expression-id : operator-function-id template-spec?")
@glrp.rule("unqualified-expression-id : conversion-function-id")
@glrp.rule("unqualified-expression-id : '~' 'identifier'")
@cxx98
def p_unqualified_expression_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("unqualified-expression-id : literal-operator-id template-spec?")
@glrp.rule("unqualified-expression-id : '~' decltype-specifier")
@cxx11
def p_unqualified_expression_id_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("qualified-expression-id : nested-name-specifier 'template'? unqualified-expression-id")
@glrp.rule("qualified-expression-id : '::' nested-name-specifier 'template'? unqualified-expression-id")
@glrp.rule("qualified-expression-id : '::' operator-function-id template-spec?")
@cxx98
def p_qualified_expression_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("qualified-expression-id : '::' literal-operator-id template-spec?")
@cxx11
def p_qualified_expression_id_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("nested-name-specifier : 'identifier' template-spec? [prec:left,2] '::'")
@glrp.rule("nested-name-specifier : nested-name-specifier 'template'? 'identifier' template-spec? '::'")
@cxx98
def p_nested_name_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("nested-name-specifier : decltype-specifier '::'")
@cxx11
def p_nested_name_specifier_decltype(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("literal : string-literal")
@glrp.rule("literal : integer-literal")
@glrp.rule("literal : floating-literal")
@cxx98
def p_literal(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("template-spec : '<[' template-argument-list? ']>'")
@cxx98
def p_template_spec(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser