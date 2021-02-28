"""
linkage-specification:
      extern string-literal { declaration-seqopt }
      extern string-literal declaration
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_linkage_specification(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        linkage-specification : attribute-specifier-seq? decl-specifier-seq STRING_LITERAL LBRACE declaration-seq? RBRACE
    """
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser