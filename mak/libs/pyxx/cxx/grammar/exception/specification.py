"""
noexcept-specifier:
    noexcept ( constant-expression )
    noexcept
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('noexcept-specifier : "noexcept" "(" constant-expression ")"')
@glrp.rule('noexcept-specifier : "noexcept"')
@cxx98
def noexcept_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser