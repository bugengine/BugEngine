from be_typing import TYPE_CHECKING
import glrp
from ...cxx_parser import cxx98


@glrp.rule("builtin-type-specifier : 'i8' | 'i16' | 'i32' | 'i64' | 'u8' | 'u16' | 'u32' | 'u64'")
@cxx98
def p_builtin_type_specifier_extension(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser