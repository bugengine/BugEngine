from be_typing import TYPE_CHECKING


def p_primary_expression_extension(p):
    # type: (YaccProduction) -> None
    """
        primary-expression : KW_TRUE
                           | KW_FALSE
                           | KW_NULLPTR
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction