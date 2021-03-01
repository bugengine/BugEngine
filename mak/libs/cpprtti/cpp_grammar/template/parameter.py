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

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_template_parameter(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-parameter : type-parameter
                           | parameter-declaration
    """


@cpp98
def p_type_parameter(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-parameter : KW_CLASS ELLIPSIS? IDENTIFIER?
                       | KW_CLASS IDENTIFIER? OP_EQUALS type-id
                       | KW_TYPENAME ELLIPSIS? IDENTIFIER?
                       | KW_TYPENAME IDENTIFIER? OP_EQUALS type-id
                       | KW_TEMPLATE LANGLE template-parameter-list RANGLE KW_CLASS ELLIPSIS? IDENTIFIER?
                       | KW_TEMPLATE LANGLE template-parameter-list RANGLE KW_CLASS IDENTIFIER? OP_EQUALS id-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser