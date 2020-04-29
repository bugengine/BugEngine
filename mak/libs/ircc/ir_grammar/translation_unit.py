from be_typing import TYPE_CHECKING


def p_translation_unit(p):
    # type: (YaccProduction) -> None
    """
        translation-unit : empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction