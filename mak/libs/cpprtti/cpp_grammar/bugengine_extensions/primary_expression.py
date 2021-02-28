from be_typing import TYPE_CHECKING
from ...cpp_parser import cpp98


@cpp98
def p_primary_expression_extension(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        primary-expression : KW_TRUE
                           | KW_FALSE
                           | KW_NULLPTR
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser