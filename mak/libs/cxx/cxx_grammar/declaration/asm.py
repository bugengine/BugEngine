"""
asm-definition:
      asm ( string-literal ) ;
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("asm-definition : attribute-specifier-seq? decl-specifier-seq? 'asm' '(' 'string-literal' ')' ';'")
@cxx98
def p_asm_definition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: no specifier


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser