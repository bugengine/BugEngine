"""
constant-expression:
      conditional-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_constant_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        constant-expression : conditional-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser