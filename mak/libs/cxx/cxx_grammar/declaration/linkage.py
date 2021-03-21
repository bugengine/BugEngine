"""
linkage-specification:
      extern string-literal { declaration-seqopt }
      extern string-literal declaration
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule(
    "linkage-specification : attribute-specifier-seq? decl-specifier-seq 'string-literal' '{' declaration-seq? '}'"
)
@cxx98
def p_linkage_specification(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser