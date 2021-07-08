"""
compound-requirement:
    { expression } noexcept? return-type-requirement? ;
return-type-requirement:
    -> type-constraint
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('compound-requirement : "{" expression "}" "noexcept"? return-type-requirement? ";"')
@cxx98
def compound_requirement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('return-type-requirement : "->" type-constraint')
@cxx98
def return_type_requirement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser
