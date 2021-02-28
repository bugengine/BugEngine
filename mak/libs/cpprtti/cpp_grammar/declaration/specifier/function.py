"""
function-specifier:
    inline
    virtual
    explicit
"""

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_function_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        function-specifier : KW_INLINE
                           | KW_VIRTUAL
                           | KW_EXPLICIT
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser