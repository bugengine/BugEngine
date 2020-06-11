from ...ir_ast.instructions import IrInstCast
from be_typing import TYPE_CHECKING


def p_ir_opcode_cast(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ir-cast-op ir-value TO ir-type ir-instruction-attachment-list
    """
    p[0] = IrInstCast(p[1], p[3], p[5], p[2], p[6])


def p_ir_cast_op(p):
    # type: (YaccProduction) -> None
    """
        ir-cast-op : ADDRSPACECAST
                   | BITCAST
                   | TRUNC
                   | PTRTOINT
                   | INTTOPTR
                   | SEXT
                   | ZEXT
                   | FPTOSI
                   | FPTOUI
                   | FPTRUNC
                   | SITOFP
                   | UITOFP
                   | FPEXT
    """
    p[0] = p[1]


if TYPE_CHECKING:
    from ply.yacc import YaccProduction