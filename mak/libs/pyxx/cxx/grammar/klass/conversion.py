"""
conversion-function-id:
    operator conversion-type-id

conversion-type-id:
    type-specifier-seq conversion-declarator?

conversion-declarator:
    ptr-operator conversion-declarator?
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('conversion-function-id : operator conversion-type-id')
@cxx98
def conversion_function_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('conversion-type-id : type-specifier-seq conversion-declarator?')
@cxx98
def conversion_type_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


#@glrp.rule('conversion-declarator : ptr-operator conversion-declarator?')
@glrp.rule('conversion-declarator[split] : ptr-operator')
@glrp.rule('conversion-declarator : ptr-operator conversion-declarator')
@cxx98
def conversion_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser