"""
typename-specifier:
      typename ::opt nested-name-specifier identifier     C++0x
      typename ::opt nested-name-specifier templateopt simple-template-id     C++0x
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("typename-specifier : 'typename' id-type")
@cxx98
def p_typename_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser