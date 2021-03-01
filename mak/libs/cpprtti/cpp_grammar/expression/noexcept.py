"""
noexcept-expression:
      noexcept ( expression )     C++0x
"""

from be_typing import TYPE_CHECKING

from ...cpp_parser import cpp11


@cpp11
def p_noexcept_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noexcept-expression : KW_NOEXCEPT LPAREN expression RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser