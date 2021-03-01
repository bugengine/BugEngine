"""
typedef-name:
      identifier
"""

from ...cpp_parser import disabled
from be_typing import TYPE_CHECKING


@disabled
def p_typedef_name(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        typedef-name : IDENTIFIER
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser