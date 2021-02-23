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
        using-declaration : attribute-specifier-seq? KW_USING KW_TYPENAME? id-expression SEMI
    """
    # TODO: attribute-specifier-seq? not allowed here


def p_using_directive(p):
    # type: (YaccProduction) -> None
    """
        using-directive : attribute-specifier-seq? KW_USING KW_NAMESPACE id-expression IDENTIFIER SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction