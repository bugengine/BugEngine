"""
literal-operator-id:
  	operator "" identifier     C++0x
"""

from be_typing import TYPE_CHECKING


def p_literal_operator_id(p):
    # type: (YaccProduction) -> None
    """
        literal-operator-id : KW_OPERATOR STRING_LITERAL identifier
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction