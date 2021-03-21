from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98, cxx11


@glrp.rule("primary-expression : 'true' | 'false'")
@cxx98
def p_primary_expression_bool(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("primary-expression : 'nullptr'")
@cxx11
def p_primary_expression_nullptr(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser