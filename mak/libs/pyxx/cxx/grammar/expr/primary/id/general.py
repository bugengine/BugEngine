"""
id-expression:
    unqualified-id
    qualified-id
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('id-expression : unqualified-id')
@glrp.rule('id-expression : qualified-id')
@cxx98
def id_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser