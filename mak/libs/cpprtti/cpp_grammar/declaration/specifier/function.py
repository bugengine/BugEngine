"""
function-specifier:
    inline
    virtual
    explicit
"""

from be_typing import TYPE_CHECKING


def p_function_specifier(p):
    # type: (YaccProduction) -> None
    """
        function-specifier : KW_INLINE
                           | KW_VIRTUAL
                           | KW_EXPLICIT
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction