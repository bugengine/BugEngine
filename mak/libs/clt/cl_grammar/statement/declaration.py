"""
declaration-statement:
      block-declaration
"""

from be_typing import TYPE_CHECKING


def p_declaration_statement(p):
    # type: (YaccProduction) -> None
    """
        declaration-statement : block-declaration
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction