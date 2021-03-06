"""
typename-specifier:
      typename ::opt nested-name-specifier identifier     C++0x
      typename ::opt nested-name-specifier templateopt simple-template-id     C++0x
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_typename_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        typename-specifier : KW_TYPENAME id-type
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser