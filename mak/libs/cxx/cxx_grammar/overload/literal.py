"""
literal-operator-id:
  	operator "" identifier     C++0x
"""

from ...cxx_parser import cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("literal-operator-id : 'operator' 'string-literal' 'identifier'")
@cxx11
def p_literal_operator_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    # TODO: string literal should be empty.
    pass


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cxx_parser import CxxParser