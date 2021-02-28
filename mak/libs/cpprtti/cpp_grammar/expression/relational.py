"""
relational-expression:
      shift-expression
      relational-expression < shift-expression
      relational-expression > shift-expression
      relational-expression <= shift-expression
      relational-expression >= shift-expression
"""

from be_typing import TYPE_CHECKING

from ...cpp_parser import cpp98


@cpp98
def p_relational_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        relational-expression : shift-expression
                              | relational-expression OP_LT shift-expression
                              | relational-expression OP_GT shift-expression
                              | relational-expression OP_LE shift-expression
                              | relational-expression OP_GE shift-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser