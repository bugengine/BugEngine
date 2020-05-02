from be_typing import TYPE_CHECKING


def p_ir_opcode_ret(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : RET ir-constant
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction