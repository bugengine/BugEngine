"""
pm-expression:
      cast-expression
      pm-expression .* cast-expression
      pm-expression ->* cast-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_pm_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        pm-expression : cast-expression
                      | pm-expression OP_PERIODMEMBER cast-expression
                      | pm-expression OP_ARROWMEMBER cast-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser