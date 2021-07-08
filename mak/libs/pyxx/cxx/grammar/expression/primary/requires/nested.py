"""
nested-requirement:
    requires constraint-expression ;
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('nested-requirement : "requires" constraint-expression ";"')
@cxx98
def nested_requirement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser
