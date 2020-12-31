"""
asm-definition:
      asm ( string-literal ) ;
"""

from be_typing import TYPE_CHECKING


def p_asm_definition(p):
    # type: (YaccProduction) -> None
    """
        asm-definition : KW_ASM LPAREN STRING_LITERAL RPAREN SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction