from be_typing import TYPE_CHECKING


def p_ir_opcode_unary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-unary-op ir-type ir-constant ir-instruction-attachment-list
    """


def p_ir_unary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-unary-op : FNEG
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction