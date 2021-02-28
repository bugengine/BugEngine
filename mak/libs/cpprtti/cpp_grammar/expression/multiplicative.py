"""
multiplicative-expression:
      pm-expression
      multiplicative-expression * pm-expression
      multiplicative-expression / pm-expression
      multiplicative-expression % pm-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_multiplicative_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        multiplicative-expression : pm-expression
                                  | multiplicative-expression OP_TIMES pm-expression
                                  | multiplicative-expression OP_DIVIDE pm-expression
                                  | multiplicative-expression OP_MOD pm-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser