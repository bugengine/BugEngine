"""
explicit-specialization:
  	template < > declaration
"""

from ....cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("explicit-specialization : attribute-specifier-seq? decl-specifier-seq? 'template' '<[' ']>' declaration")
@cxx98
def p_explicit_specialization(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: attribute empty
    # TODO: decl specifier empty


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser