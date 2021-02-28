"""
equality-expression:
      relational-expression
      equality-expression == relational-expression
      equality-expression != relational-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_equality_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        equality-expression : relational-expression
                            | equality-expression OP_EQ relational-expression
                            | equality-expression OP_NE relational-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser