from be_typing import TYPE_CHECKING


def p_translation_unit(p):
    # type: (YaccProduction) -> None
    """
        translation-unit : declaration-seq?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction