"""
typedef-name:
      identifier
"""

from be_typing import TYPE_CHECKING

#def p_typedef_name(p):
#    # type: (YaccProduction) -> None
#    """
#        typedef-name : IDENTIFIER
#    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction