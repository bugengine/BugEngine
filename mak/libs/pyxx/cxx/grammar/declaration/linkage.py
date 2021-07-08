"""
linkage-specification:
    extern string-literal { declaration-seq? }
    extern string-literal declaration
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('linkage-specification : "extern" "string-literal" "{" declaration-seq? "}"')
@glrp.rule('linkage-specification : "extern" "string-literal" declaration')
@cxx98
def linkage_specification(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser