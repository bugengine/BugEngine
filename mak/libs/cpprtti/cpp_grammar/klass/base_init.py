"""
ctor-initializer:
      : mem-initializer-list
mem-initializer-list:
      mem-initializer ...opt     C++0x
      mem-initializer , mem-initializer-list ...opt     C++0x
mem-initializer:
      mem-initializer-id ( expression-listopt )
      mem-initializer-id braced-init-list     C++0x
mem-initializer-id:
      class-or-decltype
      identifier
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_ctor_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ctor-initializer : COLON mem-initializer-list
    """


@cpp98
def p_mem_initializer_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        mem-initializer-list : mem-initializer ELLIPSIS?
                             | mem-initializer COMMA mem-initializer-list ELLIPSIS?
    """


@cpp98
def p_mem_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        mem-initializer : mem-initializer-id LPAREN expression-list? RPAREN
    """


@cpp11
def p_mem_initializer_braced(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        mem-initializer : mem-initializer-id braced-init-list
    """


@cpp98
def p_mem_initializer_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        mem-initializer-id : class-or-decltype
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser