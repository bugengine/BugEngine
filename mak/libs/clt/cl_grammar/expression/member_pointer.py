"""
pm-expression:
      cast-expression
      pm-expression .* cast-expression
      pm-expression ->* cast-expression
"""

from be_typing import TYPE_CHECKING


def p_pm_expression(p):
    # type: (YaccProduction) -> None
    """
        pm-expression : cast-expression
                      | pm-expression OP_PERIODMEMBER cast-expression
                      | pm-expression OP_ARROWMEMBER cast-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction