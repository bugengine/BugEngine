"""
typename-specifier:
      typename ::opt nested-name-specifier identifier     C++0x
      typename ::opt nested-name-specifier templateopt simple-template-id     C++0x
"""

from be_typing import TYPE_CHECKING


def p_typename_specifier(p):
    # type: (YaccProduction) -> None
    """
        typename-specifier : KW_TYPENAME id-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction