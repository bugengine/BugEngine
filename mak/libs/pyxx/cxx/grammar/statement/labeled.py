"""
labeled-statement:
    attribute-specifier-seq? identifier : statement
    attribute-specifier-seq? case constant-expression : statement
    attribute-specifier-seq? default : statement
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('labeled-statement : attribute-specifier-seq? "identifier" ":" statement')
@glrp.rule('labeled-statement : attribute-specifier-seq? "case" constant-expression ":" statement')
@glrp.rule('labeled-statement : attribute-specifier-seq? "default" ":" statement')
@cxx98
def labeled_statement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser