"""
noexcept-expression:
      noexcept ( expression )     C++0x
"""

from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx11


@glrp.rule("noexcept-expression : 'noexcept' '(' expression ')'")
@cxx11
def p_noexcept_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser