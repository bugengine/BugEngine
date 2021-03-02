"""
namespace-alias:
      identifier
namespace-alias-definition:
      namespace identifier = qualified-namespace-specifier ;
qualified-namespace-specifier:
      ::opt nested-name-specifieropt namespace-name
"""

from ...cpp_parser import cpp98, disabled
from be_typing import TYPE_CHECKING


@disabled
def p_namespace_alias(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        namespace-alias : IDENTIFIER
    """


@cpp98
def p_namespace_alias_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        namespace-alias-definition : attribute-specifier-seq? decl-specifier-seq? KW_NAMESPACE IDENTIFIER OP_EQUALS id-expression SEMI
    """
    # TODO: attributes empty
    # TODO: decl specifier none


@disabled
def p_qualified_namespace_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        qualified-namespace-specifier : id-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser