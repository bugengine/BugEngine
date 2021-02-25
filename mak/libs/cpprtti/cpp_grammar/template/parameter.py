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

from be_typing import TYPE_CHECKING


def p_template_parameter(p):
    # type: (YaccProduction) -> None
    """
        template-parameter : type-parameter
                           | parameter-declaration
    """


def p_type_parameter(p):
    # type: (YaccProduction) -> None
    """
        type-parameter : KW_CLASS ELLIPSIS? IDENTIFIER?
                       | KW_CLASS IDENTIFIER? OP_EQUALS type-id
                       | KW_TYPENAME ELLIPSIS? IDENTIFIER?
                       | KW_TYPENAME IDENTIFIER? OP_EQUALS type-id
                       | KW_TEMPLATE LANGLE template-parameter-list RANGLE KW_CLASS ELLIPSIS? IDENTIFIER?
                       | KW_TEMPLATE LANGLE template-parameter-list RANGLE KW_CLASS IDENTIFIER? OP_EQUALS id-expression
    """
    # original: template < template-parameter-list > class identifieropt = id-expression


if TYPE_CHECKING:
    from ply.yacc import YaccProduction