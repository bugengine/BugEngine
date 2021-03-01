"""
literal-operator-id:
  	operator "" identifier     C++0x
"""

from ...cpp_parser import cpp11
from be_typing import TYPE_CHECKING


@cpp11
def p_literal_operator_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        literal-operator-id : KW_OPERATOR STRING_LITERAL IDENTIFIER
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser