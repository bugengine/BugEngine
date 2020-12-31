"""
explicit-specialization:
  	template < > declaration
"""

from be_typing import TYPE_CHECKING


def p_explicit_specialization(p):
    # type: (YaccProduction) -> None
    """
        explicit-specialization : KW_TEMPLATE OP_LT OP_GT declaration
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction