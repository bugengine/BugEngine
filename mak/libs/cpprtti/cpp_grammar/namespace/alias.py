"""
namespace-alias:
      identifier
namespace-alias-definition:
      namespace identifier = qualified-namespace-specifier ;
qualified-namespace-specifier:
      ::opt nested-name-specifieropt namespace-name
"""

from be_typing import TYPE_CHECKING


def p_namespace_alias(p):
    # type: (YaccProduction) -> None
    """
        namespace-alias : IDENTIFIER
    """


def p_namespace_alias_definition(p):
    # type: (YaccProduction) -> None
    """
        namespace-alias-definition : KW_NAMESPACE identifier OP_EQUALS qualified-namespace-specifier SEMI
    """


def p_qualified_namespace_specifier(p):
    # type: (YaccProduction) -> None
    """
        qualified-namespace-specifier : scope-opt nested-name-specifier-opt namespace-name
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction