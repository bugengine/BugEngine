from be_typing import TYPE_CHECKING
from ...cpp_parser import cpp98, cpp11


@cpp98
def p_access_specifier_published(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        access-specifier : KW_PUBLISHED
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser