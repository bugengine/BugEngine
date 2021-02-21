"""
using-declaration:
      using typenameopt ::opt nested-name-specifier unqualified-id ;
      using :: unqualified-id ;
using-directive:
      attribute-specifier-seqopt using namespace ::opt nested-name-specifieropt namespace-name ;
"""

from be_typing import TYPE_CHECKING


def p_using_declaration(p):
    # type: (YaccProduction) -> None
    """
        using-declaration : KW_USING KW_TYPENAME? OP_SCOPE? nested-name-specifier unqualified-id SEMI
                          | KW_USING OP_SCOPE unqualified-id SEMI
    """


def p_using_directive(p):
    # type: (YaccProduction) -> None
    """
        using-directive : attribute-specifier-seq? KW_USING KW_NAMESPACE OP_SCOPE? nested-name-specifier? IDENTIFIER SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction