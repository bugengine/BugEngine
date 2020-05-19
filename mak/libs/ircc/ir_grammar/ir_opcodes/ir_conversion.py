from be_typing import TYPE_CHECKING


def p_ir_opcode_cast(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-cast-op ir-value TO ir-type ir-instruction-attachment-list
    """


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


if TYPE_CHECKING:
    from ply.yacc import YaccProduction