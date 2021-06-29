"""
translation-unit:
    declaration-seq?
    global-module-fragment? module-declaration declaration-seq? private-module-fragment?
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('translation-unit : declaration-seq?')
@glrp.rule('translation-unit : global-module-fragment? module-declaration declaration-seq? private-module-fragment?')
@cxx98
def translation_unit(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser