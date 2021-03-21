"""
namespace-name:
      original-namespace-name
      namespace-alias
original-namespace-name:
      identifier
namespace-definition:
      named-namespace-definition
      unnamed-namespace-definition
named-namespace-definition:
      original-namespace-definition
      extension-namespace-definition
original-namespace-definition:
      inlineopt namespace identifier { namespace-body }     C++0x
extension-namespace-definition:
      inlineopt namespace original-namespace-name { namespace-body }     C++0x
unnamed-namespace-definition:
      inlineopt namespace { namespace-body }
namespace-body:
      declaration-seqopt
"""

from ...cxx_parser import cxx98, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule(
    "namespace-definition : attribute-specifier-seq? decl-specifier-seq? 'namespace' id-type? '{' namespace-body '}'"
)
@cxx98
def p_namespace_definition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("namespace-body : declaration-seq?")
@cxx98
def p_namespace_body(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser