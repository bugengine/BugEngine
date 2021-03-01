"""
explicit-instantiation:
      externopt template declaration     C++0x
"""

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_explicit_instantiation(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        explicit-instantiation : attribute-specifier-seq? decl-specifier-seq? KW_TEMPLATE declaration
        explicit-instantiation : attribute-specifier-seq? decl-specifier-seq? KW_EXTERN KW_TEMPLATE declaration
    """
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser