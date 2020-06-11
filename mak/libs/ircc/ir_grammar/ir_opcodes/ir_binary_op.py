from ...ir_ast.instructions import IrInstBinaryOp, IrInstFloatBinaryOp
from be_typing import TYPE_CHECKING


def p_ir_opcode_binary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ir-binary-op ir-binary-op-modifier-opt ir-type ir-expr COMMA ir-expr ir-instruction-attachment-list
    """
    p[0] = IrInstBinaryOp(p[1], p[4], p[5], p[7], p[2], p[8])


def p_ir_binary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-op : ADD
                     | SUB
                     | MUL
                     | UDIV
                     | SDIV
                     | UREM
                     | SREM
                     | SHL
                     | LSHR
                     | ASHR
                     | AND
                     | OR
                     | XOR
    """
    p[0] = p[1]


def p_ir_binary_op_modifier_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-op-modifier-opt : NUW
                                  | NSW
                                  | NUW NSW
                                  | empty
    """


def p_ir_opcode_binary_fop(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ir-binary-fop ir-binary-fop-modifier-opt ir-type ir-expr COMMA ir-expr ir-instruction-attachment-list
    """
    p[0] = IrInstFloatBinaryOp(p[1], p[4], p[5], p[7], p[2], p[8])


def p_ir_binary_fop(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-fop : FADD
                      | FSUB
                      | FMUL
                      | FDIV
                      | FREM
    """
    p[0] = p[1]


def p_ir_binary_fop_modifier_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-fop-modifier-opt : empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction