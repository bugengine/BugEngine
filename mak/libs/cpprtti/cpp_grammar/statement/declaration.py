"""
declaration-statement:
      block-declaration
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_declaration_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declaration-statement : block-declaration
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser