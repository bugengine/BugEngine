"""
template-parameter:
      type-parameter
      parameter-declaration
type-parameter:
      class ...opt identifieropt     C++0x
      class identifieropt = type-id
      typename ...opt identifieropt     C++0x
      typename identifieropt = type-id
      template < template-parameter-list > class ...opt identifieropt     C++0x
      template < template-parameter-list > class identifieropt = id-expression
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("template-parameter : type-parameter | parameter-declaration")
@cxx98
def p_template_parameter(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("type-parameter : 'class' 'identifier'?")
@glrp.rule("type-parameter : 'class' 'identifier'? '=' type-id")
@glrp.rule("type-parameter : 'typename' 'identifier'?")
@glrp.rule("type-parameter : 'typename' 'identifier'? '=' type-id")
@glrp.rule("type-parameter : 'template' '<[' template-parameter-list ']>' 'class' 'identifier'?")
@glrp.rule("type-parameter : 'template' '<[' template-parameter-list ']>' 'class' 'identifier'? '=' type-id")
@cxx98
def p_type_parameter(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("type-parameter : 'class' '...' 'identifier'?")
@glrp.rule("type-parameter : 'typename' '...' 'identifier'?")
@glrp.rule("type-parameter : 'template' '<[' template-parameter-list ']>' 'class' '...' 'identifier'? '=' type-id")
@cxx98
def p_type_parameter_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser