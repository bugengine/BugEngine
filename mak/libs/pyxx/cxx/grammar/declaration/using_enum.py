"""
using-enum-declaration:
    using elaborated-enum-specifier ;
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('using-enum-declaration : "using" elaborated-enum-specifier ";"')
@cxx98
def using_enum_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser