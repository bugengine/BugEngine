"""
namespace-alias:
      identifier
namespace-alias-definition:
      namespace identifier = qualified-namespace-specifier ;
qualified-namespace-specifier:
      ::opt nested-name-specifieropt namespace-name
"""

from ...cxx_parser import cxx98, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule(
    "namespace-alias-definition : attribute-specifier-seq? decl-specifier-seq? 'namespace' 'identifier' '=' id-type ';'"
)
@cxx98
def p_namespace_alias_definition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        
    """
    # TODO: attributes empty
    # TODO: decl specifier none
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser