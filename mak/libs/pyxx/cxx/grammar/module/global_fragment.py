"""
global-module-fragment:
    module-keyword ; declaration-seq?
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('global-module-fragment : "module" ";" declaration-seq?')
@cxx98
def global_module_fragment(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser