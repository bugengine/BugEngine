"""
exception-specification:
      dynamic-exception-specification     C++0x
      noexcept-specification     C++0x
dynamic-exception-specification:
      throw ( type-id-listopt )     C++0x
type-id-list:
      type-id ...opt     C++0x
      type-id-list , type-id ...opt     C++0x
noexcept-specification:
      noexcept ( constant-expression )     C++0x
      noexcept     C++0x
"""

from be_typing import TYPE_CHECKING


def p_exception_specification(p):
    # type: (YaccProduction) -> None
    """
        exception-specification : dynamic-exception-specification
                                | noexcept-specification
    """


def p_dynamic_exception_specification(p):
    # type: (YaccProduction) -> None
    """
        dynamic-exception-specification : KW_THROW LPAREN type-id-list? RPAREN
    """


def p_type_id_list(p):
    # type: (YaccProduction) -> None
    """
        type-id-list : type-id ELLIPSIS?
                     | type-id-list COMMA type-id ELLIPSIS?
    """


def p_noexcept_specification(p):
    # type: (YaccProduction) -> None
    """
        noexcept-specification : KW_NOEXCEPT LPAREN constant-expression RPAREN
                               | KW_NOEXCEPT
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction