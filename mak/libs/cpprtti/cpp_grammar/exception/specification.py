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

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_exception_specification(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        exception-specification : dynamic-exception-specification
    """


@cpp11
def p_exception_specification_noexcept(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        exception-specification : noexcept-specification
    """


@cpp98
def p_dynamic_exception_specification(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        dynamic-exception-specification : KW_THROW LPAREN type-id-list? RPAREN
    """


@cpp98
def p_type_id_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-id-list : type-id ELLIPSIS?
                     | type-id-list COMMA type-id ELLIPSIS?
    """


@cpp11
def p_noexcept_specification(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noexcept-specification : KW_NOEXCEPT LPAREN constant-expression RPAREN
                               | KW_NOEXCEPT
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser