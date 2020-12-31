"""
noexcept-expression:
      noexcept ( expression )     C++0x
"""

from be_typing import TYPE_CHECKING


def p_noexcept_expression(p):
    # type: (YaccProduction) -> None
    """
        noexcept-expression : KW_NOEXCEPT LPAREN expression RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction