"""
concept-definition:
    concept concept-name = constraint-expression ;

concept-name:
    identifier
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('concept-definition : "concept" concept-name "=" constraint-expression ";"')
@cxx98
def concept_definition(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('concept-name : "identifier"')
@cxx98
def concept_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser