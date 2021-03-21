"""
function-specifier:
    inline
    virtual
    explicit
"""

from ....cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("function-specifier : 'inline' | 'virtual' | 'explicit'")
@cxx98
def p_function_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser