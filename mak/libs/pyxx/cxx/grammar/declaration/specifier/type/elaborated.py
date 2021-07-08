"""
elaborated-type-specifier:
    class-key attribute-specifier-seq? nested-name-specifier? identifier
    class-key simple-template-id
    class-key nested-name-specifier template? simple-template-id
    elaborated-enum-specifier

elaborated-enum-specifier:
    enum nested-name-specifier? identifier
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('elaborated-type-specifier : class-key attribute-specifier-seq? nested-name-specifier? "identifier"')
@glrp.rule('elaborated-type-specifier : class-key simple-template-id')
@glrp.rule('elaborated-type-specifier : class-key nested-name-specifier "template"? simple-template-id')
@glrp.rule('elaborated-type-specifier : elaborated-enum-specifier')
@cxx98
def elaborated_type_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('elaborated-enum-specifier : enum nested-name-specifier? "identifier"')
@cxx98
def elaborated_enum_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser