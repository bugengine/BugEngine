"""
explicit-instantiation:
      externopt template declaration     C++0x
"""

from be_typing import TYPE_CHECKING


def p_explicit_instantiation(p):
    # type: (YaccProduction) -> None
    """
        explicit-instantiation : KW_EXTERN KW_TEMPLATE declaration
                               | KW_TEMPLATE declaration
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction