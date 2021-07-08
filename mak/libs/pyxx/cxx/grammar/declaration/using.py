"""
using-declaration:
    using using-declarator-list ;

using-declarator-list:
    using-declarator ...?
    using-declarator-list , using-declarator ...?

using-declarator:
    typename? nested-name-specifier unqualified-id
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('using-declaration : "using" using-declarator-list ";"')
@cxx98
def using_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('using-declarator-list : using-declarator "..."?')
@glrp.rule('using-declarator-list : using-declarator-list "," using-declarator "..."?')
@cxx98
def using_declarator_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('using-declarator : "typename"? nested-name-specifier unqualified-id')
@cxx98
def using_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser