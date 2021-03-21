"""
jump-statement:
      break ;
      continue ;
      return expressionopt ;
      return braced-init-listopt ;     C++0x
      goto identifier ;
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("jump-statement : 'break' ';' | 'continue' ';' | 'return' expression? ';' | 'goto' 'identifier' ';'")
@cxx98
def p_jump_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("jump-statement : 'return' braced-init-list ';'")
@cxx11
def p_jump_statement_braced(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser