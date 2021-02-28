from be_typing import TYPE_CHECKING
from ...cpp_parser import cpp98


@cpp98
def p_translation_unit(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        translation-unit : declaration-seq?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser