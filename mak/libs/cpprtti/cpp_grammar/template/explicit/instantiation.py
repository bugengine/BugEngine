"""
explicit-instantiation:
      externopt template declaration     C++0x
"""

from be_typing import TYPE_CHECKING


def p_explicit_instantiation(p):
    # type: (YaccProduction) -> None
    """
        explicit-instantiation : extern-opt KW_TEMPLATE declaration
    """


def p_extern_opt(p):
    # type: (YaccProduction) -> None
    """
        extern-opt : KW_EXTERN
                   | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction