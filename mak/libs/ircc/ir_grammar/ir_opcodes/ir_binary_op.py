from be_typing import TYPE_CHECKING


def p_ir_opcode_binary_op(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-binary-op ir-binary-op-modifier-opt ir-type ir-value COMMA ir-value ir-instruction-attachment-list
    """


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
        ir-opcode : ir-binary-fop ir-binary-fop-modifier-opt ir-type ir-value COMMA ir-value ir-instruction-attachment-list
    """


def p_ir_binary_fop(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-fop : FADD
                      | FSUB
                      | FMUL
                      | FDIV
                      | FREM
    """


def p_ir_binary_fop_modifier_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-binary-fop-modifier-opt : empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction