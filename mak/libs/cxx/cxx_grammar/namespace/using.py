"""
using-declaration:
      using typenameopt ::opt nested-name-specifier unqualified-id ;
      using :: unqualified-id ;
using-directive:
      attribute-specifier-seqopt using namespace ::opt nested-name-specifieropt namespace-name ;
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("using-declaration : attribute-specifier-seq? decl-specifier-seq? 'using' 'typename'? id-type ';'")
@cxx98
def p_using_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    # TODO: attribute-specifier-seq? not allowed here
    # TODO: decl none
    pass


@glrp.rule("using-directive : attribute-specifier-seq? decl-specifier-seq? 'using' 'namespace' id-type ';'")
@cxx98
def p_using_directive(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    # TODO: decl none
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser