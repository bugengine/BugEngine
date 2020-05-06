from be_typing import TYPE_CHECKING


def p_ir_opcode_ret(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : RET ir-type ir-value ir-instruction-attachment-list
                  | RET VOID ir-instruction-attachment-list
    """


def p_ir_opcode_br(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : BR LABEL ir-value ir-instruction-attachment-list
                  | BR ir-type ir-value COMMA LABEL ir-value COMMA LABEL ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_switch(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SWITCH ir-type ir-value COMMA LABEL ir-value LBRACKET ir-switch-target-list RBRACKET ir-instruction-attachment-list
    """


def p_ir_switch_target_list(p):
    # type: (YaccProduction) -> None
    """
        ir-switch-target-list : ir-type ir-value COMMA LABEL ir-value ir-switch-target-list
                              | empty
    """


def p_ir_opcode_unreachable(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : UNREACHABLE ir-instruction-attachment-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction