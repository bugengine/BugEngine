from ...ir_ast.instructions import IrInstBranch, IrInstConditionalBranch, IrInstRet, IrInstSwitch, IrInstUnreachable
from be_typing import TYPE_CHECKING


def p_ir_opcode_ret(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : RET ir-value ir-instruction-attachment-list
    """
    p[0] = IrInstRet(p[2], p[3])


def p_ir_opcode_br(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : BR LABEL ID ir-instruction-attachment-list
    """
    p[0] = IrInstBranch(p[3], p[4])


def p_ir_opcode_br_conditional(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : BR ir-value COMMA LABEL ID COMMA LABEL ID ir-instruction-attachment-list
    """
    p[0] = IrInstConditionalBranch(p[2], p[5], p[8], p[9])


def p_ir_opcode_switch(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SWITCH ir-value COMMA LABEL ID LBRACKET ir-switch-target-list RBRACKET ir-instruction-attachment-list
    """
    p[0] = IrInstSwitch(p[2], p[5], p[7], p[9])


def p_ir_switch_target_list(p):
    # type: (YaccProduction) -> None
    """
        ir-switch-target-list : ir-value COMMA LABEL ID ir-switch-target-list
    """
    p[0] = [(p[1], p[4])] + p[5]


def p_ir_switch_target_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-switch-target-list : empty
    """
    p[0] = []


def p_ir_opcode_unreachable(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : UNREACHABLE ir-instruction-attachment-list
    """
    p[0] = IrInstUnreachable(p[9])


if TYPE_CHECKING:
    from ply.yacc import YaccProduction