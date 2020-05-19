from be_typing import TYPE_CHECKING


def p_ir_opcode_ret(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : RET ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_br(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : BR LABEL ir-expr ir-instruction-attachment-list
                  | BR ir-value COMMA LABEL ir-expr COMMA LABEL ir-expr ir-instruction-attachment-list
    """


def p_ir_opcode_switch(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SWITCH ir-value COMMA LABEL ir-expr LBRACKET ir-switch-target-list RBRACKET ir-instruction-attachment-list
    """


def p_ir_switch_target_list(p):
    # type: (YaccProduction) -> None
    """
        ir-switch-target-list : ir-value COMMA LABEL ir-expr ir-switch-target-list
                              | empty
    """


def p_ir_opcode_unreachable(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : UNREACHABLE ir-instruction-attachment-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction