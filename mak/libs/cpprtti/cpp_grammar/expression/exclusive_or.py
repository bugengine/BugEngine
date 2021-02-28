"""
exclusive-or-expression:
      and-expression
      exclusive-or-expression ^ and-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_exclusive_or_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        exclusive-or-expression : and-expression
                                | exclusive-or-expression OP_XOR and-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser