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

from be_typing import TYPE_CHECKING


def p_ctor_initializer(p):
    # type: (YaccProduction) -> None
    """
        ctor-initializer : COLON mem-initializer-list
    """


def p_mem_initializer_list(p):
    # type: (YaccProduction) -> None
    """
        mem-initializer-list : mem-initializer ELLIPSIS?
                             | mem-initializer COMMA mem-initializer-list ELLIPSIS?
    """


def p_mem_initializer(p):
    # type: (YaccProduction) -> None
    """
        mem-initializer : mem-initializer-id LPAREN expression-list? RPAREN
                        | mem-initializer-id braced-init-list
    """


def p_mem_initializer_id(p):
    # type: (YaccProduction) -> None
    """
        mem-initializer-id : class-or-decltype
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction