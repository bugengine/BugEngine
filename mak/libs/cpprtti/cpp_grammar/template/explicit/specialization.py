"""
explicit-specialization:
  	template < > declaration
"""

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_explicit_specialization(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        explicit-specialization : attribute-specifier-seq? decl-specifier-seq? KW_TEMPLATE LANGLE RANGLE declaration
    """
    # TODO: attribute empty
    # TODO: decl specifier empty


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser