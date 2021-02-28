"""
jump-statement:
      break ;
      continue ;
      return expressionopt ;
      return braced-init-listopt ;     C++0x
      goto identifier ;
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_jump_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
    jump-statement : KW_BREAK SEMI
                   | KW_CONTINUE SEMI
                   | KW_RETURN expression? SEMI
                   | KW_RETURN braced-init-list SEMI
                   | KW_GOTO IDENTIFIER SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser