from ...ir_ast.instructions import IrInstCall, IrInstIntegerCompare, IrInstFloatCompare, IrInstPhi, IrInstSelect
from be_typing import TYPE_CHECKING


def p_ir_opcode_call(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment-opt ir-tail-type CALL ir-calling-convention ir-parameter-attribute-list-opt ir-return-type ID LPAREN LPAREN_MARK ir-argument-list RPAREN ir-attribute-list-opt ir-instruction-attachment-list
    """
    p[0] = IrInstCall(p[1], p[13])


def p_ir_opcode_cmp(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ICMP ir-cmp-op ir-type ir-expr COMMA ir-expr ir-instruction-attachment-list
    """
    p[0] = IrInstIntegerCompare(p[1], p[4], p[5], p[6], p[3], p[8])


def p_ir_opcode_fcmp(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment FCMP ir-fcmp-op ir-type ir-expr COMMA ir-expr ir-instruction-attachment-list
    """
    p[0] = IrInstFloatCompare(p[1], p[4], p[5], p[6], p[3], p[8])


def p_ir_opcode_phi(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment PHI ir-type ir-phi-attachment-list
    """
    p[0] = IrInstPhi(p[1], p[3], p[4][0], p[4][1])


def p_ir_opcode_select(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment SELECT ir-value COMMA ir-value COMMA ir-value ir-instruction-attachment-list
    """
    p[0] = IrInstSelect(p[1], p[3], p[5], p[7], p[8])


def p_ir_tail_type(p):
    # type: (YaccProduction) -> None
    """
        ir-tail-type : TAIL
                     | MUSTTAIL
                     | NOTAIL
                     | empty
    """


def p_ir_cmp_op(p):
    # type: (YaccProduction) -> None
    """
        ir-cmp-op : EQ
                  | NE
                  | UGT
                  | UGE
                  | ULT
                  | ULE
                  | SGT
                  | SGE
                  | SLT
                  | SLE
    """
    p[0] = p[1]


def p_ir_phi_attachment_list(p):
    # type: (YaccProduction) -> None
    """
        ir-phi-attachment-list : LBRACKET ir-expr COMMA ID RBRACKET COMMA ir-phi-attachment-list
    """
    p[0] = [[(p[2], p[4])] + p[7][0], p[7][1]]


def p_ir_phi_attachment_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-phi-attachment-list : LBRACKET ir-expr COMMA ID RBRACKET ir-instruction-attachment-list
    """
    p[0] = [[(p[2], p[4])], p[6]]


def p_ir_fcmp_op(p):
    # type: (YaccProduction) -> None
    """
        ir-fcmp-op : FALSE
                   | OEQ
                   | OGT
                   | OGE
                   | OLT
                   | OLE
                   | ONE
                   | ORD
                   | UEQ
                   | UGT
                   | UGE
                   | ULT
                   | ULE
                   | UNE
                   | UNO
                   | TRUE
    """
    p[0] = p[1]


def p_ir_argument_list(p):
    # type: (YaccProduction) -> None
    """
        ir-argument-list : ir-value COMMA ir-argument-list
    """
    p[0] = [p[1]] + p[3]


def p_ir_argument_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-argument-list : ir-value
                         | empty
    """
    p[0] = [p[1]] if p[1] is not None else []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction