from ...ir_ast.instructions import IrInstFloatUnaryOp
from be_typing import TYPE_CHECKING


def p_ir_opcode_unary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ir-unary-op ir-type ir-expr ir-instruction-attachment-list
    """
    p[0] = IrInstFloatUnaryOp(p[1], p[3], p[4], p[2], p[5])


def p_ir_unary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-unary-op : FNEG
    """
    p[0] = p[1]


if TYPE_CHECKING:
    from ply.yacc import YaccProduction