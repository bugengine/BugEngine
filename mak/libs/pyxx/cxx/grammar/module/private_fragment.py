"""
private-module-fragment:
    module-keyword : private ; declaration-seq?
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('private-module-fragment : "module" ":" "private" ";" declaration-seq?')
@cxx98
def private_module_fragment(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser