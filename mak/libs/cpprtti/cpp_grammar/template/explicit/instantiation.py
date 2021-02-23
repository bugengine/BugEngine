"""
explicit-instantiation:
      externopt template declaration     C++0x
"""

from be_typing import TYPE_CHECKING


def p_explicit_instantiation(p):
    # type: (YaccProduction) -> None
    """
        explicit-instantiation : attribute-specifier-seq? decl-specifier-seq? KW_TEMPLATE declaration
    """
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ply.yacc import YaccProduction