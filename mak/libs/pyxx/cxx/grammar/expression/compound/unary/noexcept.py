"""
noexcept-expression:
    noexcept ( expression )
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('noexcept-expression : "noexcept" "(" expression ")"')
@cxx98
def noexcept_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser