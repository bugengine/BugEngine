"""
typedef-name:
    identifier
    simple-template-id
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('typedef-name[split] : [split]"identifier"')
@glrp.rule('typedef-name[split] : [split]simple-template-id')
@cxx98
def typedef_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser