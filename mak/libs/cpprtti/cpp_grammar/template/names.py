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

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING

#def p_simple_template_id(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        simple-template-id : IDENTIFIER OP_LT template-argument-list? OP_GT
#    """

#def p_template_id(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        template-id : simple-template-id
#                    | operator-function-id OP_LT template-argument-list? OP_GT
#                    | literal-operator-id OP_LT template-argument-list? OP_GT
#    """

#def p_template_name(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        template-name : IDENTIFIER
#    """


@cpp98
def p_template_argument_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-argument-list : template-argument ELLIPSIS?
                               | template-argument-list COMMA template-argument ELLIPSIS?
    """


@cpp98
def p_template_argument(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-argument : constant-expression
                          | type-id
    """
    # id-expression is included in constant-expression


@cpp98
def p_template_argument(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-argument : constant-expression
    """
    # id-expression is included in constant-expression


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser