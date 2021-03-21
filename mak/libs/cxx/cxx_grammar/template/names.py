"""
simple-template-id:
      template-name < template-argument-listopt >     C++0x
template-id:
      simple-template-id     C++0x
      operator-function-id < template-argument-listopt >     C++0x
      literal-operator-id < template-argument-listopt >     C++0x
template-name:
      identifier
template-argument-list:
      template-argument ...opt     C++0x
      template-argument-list , template-argument ...opt     C++0x
template-argument:
      constant-expression     C++0x
      type-id     C++0x
      id-expression     C++0x
"""

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@disabled
def p_simple_template_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        simple-template-id : template-name OP_LT template-argument-list? OP_GT
    """


@disabled
def p_template_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        template-id : simple-template-id
                    | operator-function-id OP_LT template-argument-list? OP_GT
                    | literal-operator-id OP_LT template-argument-list? OP_GT
    """


@disabled
def p_template_name(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        template-name : IDENTIFIER
    """


@glrp.rule("template-argument-list : template-argument")
@glrp.rule("template-argument-list : template-argument-list ',' template-argument")
@cxx98
def p_template_argument_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("template-argument-list : template-argument '...'")
@glrp.rule("template-argument-list : template-argument-list ',' template-argument '...'")
@cxx11
def p_template_argument_list_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("template-argument : constant-expression | type-id")
@cxx98
def p_template_argument(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        template-argument : 
    """
    # id-expression is included in constant-expression


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser