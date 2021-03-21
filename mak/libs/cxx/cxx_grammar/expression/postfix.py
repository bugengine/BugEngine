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

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("postfix-expression : primary-expression")
@glrp.rule("postfix-expression : postfix-expression '[' expression ']'")
@glrp.rule("postfix-expression : postfix-expression '(' expression-list? ')'")
@glrp.rule("postfix-expression : builtin-type-specifier '(' expression-list? ')'")
@glrp.rule("postfix-expression : simple-type-specifier '(' expression-list? ')'")
@glrp.rule("postfix-expression : typename-specifier '(' expression-list? ')'")
@glrp.rule("postfix-expression : postfix-expression '.' 'template'? id-expression")
@glrp.rule("postfix-expression : postfix-expression '->' 'template'? id-expression")
@glrp.rule("postfix-expression : postfix-expression '.' pseudo-destructor-name")
@glrp.rule("postfix-expression : postfix-expression '->' pseudo-destructor-name")
@glrp.rule("postfix-expression : postfix-expression '++'")
@glrp.rule("postfix-expression : postfix-expression '--'")
@glrp.rule("postfix-expression : 'dynamic_cast' '<[' type-id ']>' '(' expression ')'")
@glrp.rule("postfix-expression : 'static_cast' '<[' type-id ']>' '(' expression ')'")
@glrp.rule("postfix-expression : 'reinterpret_cast' '<[' type-id ']>' '(' expression ')'")
@glrp.rule("postfix-expression : 'const_cast' '<[' type-id ']>' '(' expression ')'")
@glrp.rule("postfix-expression : 'typeid' '(' expression ')'")
@glrp.rule("postfix-expression : 'typeid' '(' type-id ')'")
@cxx98
def p_postfix_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("postfix-expression : postfix-expression '[' braced-init-list? ']'")
@glrp.rule("postfix-expression : simple-type-specifier braced-init-list")
@glrp.rule("postfix-expression : typename-specifier braced-init-list")
@cxx11
def p_postfix_expression_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("expression-list : initializer-list")
@cxx98
def p_expression_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("pseudo-destructor-name : '::'? nested-name-specifier? '~' 'identifier'")
@cxx98
def p_pseudo_destructor_name(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("pseudo-destructor-name : '~' decltype-specifier")
@cxx11
def p_pseudo_destructor_name_cpp11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser