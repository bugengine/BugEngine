"""
explicit-specialization:
    template < > declaration

explicit-instantiation:
    extern? template declaration
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('explicit-specialization : "template" "<" ">" declaration')
@cxx98
def explicit_specialization(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('explicit-instantiation : "extern"? "template" declaration')
@cxx98
def explicit_instantiation(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser