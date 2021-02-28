"""
inclusive-or-expression:
      exclusive-or-expression
      inclusive-or-expression | exclusive-or-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_inclusive_or_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        inclusive-or-expression : exclusive-or-expression
                                | inclusive-or-expression OP_OR exclusive-or-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser