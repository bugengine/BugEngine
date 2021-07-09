"""
literal-operator-id:
    operator string-literal identifier
    operator user-defined-string-literal
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('literal-operator-id : "operator" "string-literal" "identifier"')
@glrp.rule('literal-operator-id : "operator" user-defined-string-literal')
@cxx98
def literal_operator_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser