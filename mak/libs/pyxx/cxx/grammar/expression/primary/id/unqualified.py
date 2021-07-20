"""
unqualified-id:
    identifier
    operator-function-id
    conversion-function-id
    literal-operator-id
    ~ type-name
    ~ decltype-specifier
    template-id
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('unqualified-id[split] : "identifier"')
@glrp.rule('unqualified-id[split] : operator-function-id')
@glrp.rule('unqualified-id : conversion-function-id')
@glrp.rule('unqualified-id[split] : literal-operator-id')
@glrp.rule('unqualified-id : "~" type-name')
@glrp.rule('unqualified-id : "~" decltype-specifier')
@glrp.rule('unqualified-id : template-id')
@cxx98
def unqualified_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser