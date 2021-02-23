"""
linkage-specification:
      extern string-literal { declaration-seqopt }
      extern string-literal declaration
"""

from be_typing import TYPE_CHECKING


def p_linkage_specification(p):
    # type: (YaccProduction) -> None
    """
        linkage-specification : attribute-specifier-seq? decl-specifier-seq STRING_LITERAL LBRACE declaration-seq? RBRACE
    """
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ply.yacc import YaccProduction