"""
export-declaration:
    export declaration
    export { declaration-seq? }
    export-keyword module-import-declaration
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('export-declaration : [split]"export" declaration')
@glrp.rule('export-declaration : [split]"export" "{" declaration-seq? "}"')
@glrp.rule('export-declaration[prec:right,1] : [split]"export" module-import-declaration')
@cxx98
def export_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser