"""
typedef-name:
      identifier
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING

#def p_typedef_name(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        typedef-name : IDENTIFIER
#    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser