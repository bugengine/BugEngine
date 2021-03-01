from be_typing import TYPE_CHECKING
from ...cpp_parser import cpp98


@cpp98
def p_builtin_type_specifier_extension(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        simple-type-specifier : KW_I8
                              | KW_I32
                              | KW_I64
                              | KW_U8
                              | KW_U16
                              | KW_U32
                              | KW_I16
                              | KW_U64
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser