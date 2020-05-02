from be_typing import TYPE_CHECKING
from ..ir_ast import IrModule


def p_translation_unit(p):
    # type: (YaccProduction) -> None
    """
        translation-unit : ir-header ir-declaration-list
    """
    p[0] = IrModule()


if TYPE_CHECKING:
    from ply.yacc import YaccProduction