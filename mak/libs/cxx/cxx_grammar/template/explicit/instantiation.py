"""
explicit-instantiation:
      externopt template declaration     C++0x
"""

from ....cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("explicit-instantiation : attribute-specifier-seq? decl-specifier-seq? 'template' declaration")
@cxx98
def p_explicit_instantiation(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: no attributes
    # TODO: only extern


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser