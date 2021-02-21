"""
jump-statement:
      break ;
      continue ;
      return expressionopt ;
      return braced-init-listopt ;     C++0x
      goto identifier ;
"""

from be_typing import TYPE_CHECKING


def p_jump_statement(p):
    # type: (YaccProduction) -> None
    """
    jump-statement : KW_BREAK SEMI
                   | KW_CONTINUE SEMI
                   | KW_RETURN expression-opt SEMI
                   | KW_RETURN braced-init-list SEMI
                   | KW_GOTO identifier SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction