"""
using-declaration:
      using typenameopt ::opt nested-name-specifier unqualified-id ;
      using :: unqualified-id ;
using-directive:
      attribute-specifier-seqopt using namespace ::opt nested-name-specifieropt namespace-name ;
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_using_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        using-declaration : attribute-specifier-seq? decl-specifier-seq? KW_USING KW_TYPENAME? id-expression SEMI
    """
    # TODO: attribute-specifier-seq? not allowed here
    # TODO: decl none


@cpp98
def p_using_directive(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        using-directive : attribute-specifier-seq? decl-specifier-seq? KW_USING KW_NAMESPACE id-expression SEMI
    """
    # TODO: decl none


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser