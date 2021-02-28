"""
asm-definition:
      asm ( string-literal ) ;
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_asm_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        asm-definition : KW_ASM LPAREN STRING_LITERAL RPAREN SEMI
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser