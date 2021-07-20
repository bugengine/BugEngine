"""
decltype-specifier:
    decltype ( expression )
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('decltype-specifier : [split]"decltype" "(" expression ")"')
@cxx98
def decltype_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser