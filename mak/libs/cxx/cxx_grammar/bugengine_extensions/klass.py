from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98, cxx11


@glrp.rule("access-specifier : 'published'")
@cxx98
def p_access_specifier_published(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser