"""
shift-expression:
      additive-expression
      shift-expression << additive-expression
      shift-expression >> additive-expression
"""

from be_typing import TYPE_CHECKING

from ...cpp_parser import cpp98


@cpp98
def p_shift_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        shift-expression : additive-expression
                         | shift-expression OP_LSHIFT additive-expression
                         | shift-expression OP_RSHIFT additive-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser