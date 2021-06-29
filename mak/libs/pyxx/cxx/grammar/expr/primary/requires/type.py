"""
type-requirement:
    typename nested-name-specifier? type-name ;
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('type-requirement : nested-name-specifier? type-name ";"')
@cxx98
def type_requirement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser
