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
        using-declaration : KW_USING typename-opt scope-opt nested-name-specifier unqualified-id SEMI
                          | KW_USING OP_SCOPE unqualified-id SEMI
    """


def p_using_directive(p):
    # type: (YaccProduction) -> None
    """
        using-directive : attribute-specifier-seq-opt KW_USING KW_NAMESPACE scope-opt nested-name-specifier-opt namespace-name SEMI
    """


def p_typename_opt(p):
    # type: (YaccProduction) -> None
    """
        typename-opt : KW_TYPENAME
                     | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction