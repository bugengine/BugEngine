"""
additive-expression:
      multiplicative-expression
      additive-expression + multiplicative-expression
      additive-expression - multiplicative-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_additive_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        additive-expression : multiplicative-expression
                            | additive-expression OP_PLUS multiplicative-expression
                            | additive-expression OP_MINUS multiplicative-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser