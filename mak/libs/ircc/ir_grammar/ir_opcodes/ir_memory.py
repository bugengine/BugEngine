from be_typing import TYPE_CHECKING


def p_ir_opcode_alloca(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ALLOCA ir-alloca-in ir-type
    """


def p_ir_alloca_in(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-in : INALLOCA
                     | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction