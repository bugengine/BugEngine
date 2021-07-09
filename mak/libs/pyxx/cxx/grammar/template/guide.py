"""
deduction-guide:
    explicit-specifier? template-name ( parameter-declaration-clause ) -> simple-template-id ;
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule(
    'deduction-guide : explicit-specifier? template-name "(" parameter-declaration-clause ")" "->" simple-template-id ";"'
)
@cxx98
def deduction_guide(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser