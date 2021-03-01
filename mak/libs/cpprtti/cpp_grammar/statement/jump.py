"""
jump-statement:
      break ;
      continue ;
      return expressionopt ;
      return braced-init-listopt ;     C++0x
      goto identifier ;
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_jump_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        jump-statement : KW_BREAK SEMI
                       | KW_CONTINUE SEMI
                       | KW_RETURN expression? SEMI
                       | KW_GOTO IDENTIFIER SEMI
    """


@cpp11
def p_jump_statement_braced(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        jump-statement : KW_RETURN braced-init-list SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser