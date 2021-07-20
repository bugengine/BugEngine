"""
placeholder-type-specifier:
    type-constraint? auto
    type-constraint? decltype ( auto )
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


#@glrp.rule('placeholder-type-specifier : type-constraint? "auto"')
@glrp.rule('placeholder-type-specifier : [split]"auto"')
@glrp.rule('placeholder-type-specifier : type-constraint "auto"')
@glrp.rule('placeholder-type-specifier : type-constraint? "decltype" "(" "auto" ")"')
@cxx98
def placeholder_type_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser