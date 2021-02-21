"""
linkage-specification:
      extern string-literal { declaration-seqopt }
      extern string-literal declaration
"""

from be_typing import TYPE_CHECKING


def p_linkage_specification(p):
    # type: (YaccProduction) -> None
    """
        linkage-specification : KW_EXTERN STRING_LITERAL LBRACE declaration-seq? RBRACE
                              | KW_EXTERN STRING_LITERAL declaration
"""


if TYPE_CHECKING:
    from ply.yacc import YaccProduction