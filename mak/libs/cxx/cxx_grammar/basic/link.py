from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98


@glrp.rule("translation-unit : declaration-seq? '<eof>'")
@cxx98
def p_translation_unit(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser