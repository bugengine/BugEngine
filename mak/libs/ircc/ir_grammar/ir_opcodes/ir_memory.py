from ...ir_ast.instructions import IrInstAlloca, IrInstLoad, IrInstStore, IrInstGetElementPtr
from be_typing import TYPE_CHECKING


def p_ir_opcode_alloca(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment ALLOCA ir-inalloca-opt ir-type ir-alloca-attachment-list
    """
    p[0] = IrInstAlloca(p[1], p[4], p[5][0], p[5][1], p[5][2], p[5][3])


def p_ir_opcode_load(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment LOAD ir-volatile-opt ir-type COMMA ir-value ir-loadstore-attachment-list 
    """
    p[0] = IrInstLoad(p[1], p[6], p[4], p[7][1])


def p_ir_opcode_store(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : STORE ir-volatile-opt ir-value COMMA ir-value ir-loadstore-attachment-list 
    """
    p[0] = IrInstStore(p[5], p[3], p[6][1])


def p_ir_opcode_getelementptr(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment GETELEMENTPTR ir-inbounds-opt ir-type ir-access-list 
    """
    p[0] = IrInstGetElementPtr(p[1], p[4], p[5][0], p[5][1])


def p_ir_alloca_attachment_list_count(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-attachment-list : COMMA ir-value ir-alloca-align-attachment
    """
    p[0] = [p[2]] + p[3]


def p_ir_alloca_attachment_list(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-attachment-list : ir-alloca-align-attachment
    """
    p[0] = [None] + p[1]


def p_ir_alloca_align_attachment(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-align-attachment : COMMA ALIGN ir-literal-decimal ir-alloca-addrspace-attachment
    """
    p[0] = [p[3]] + p[4]


def p_ir_alloca_noalign(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-align-attachment : ir-alloca-addrspace-attachment
    """
    p[0] = [None] + p[1]


def p_ir_alloca_addrspace(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-addrspace-attachment : COMMA ir-addrspace ir-instruction-attachment-list
    """
    p[0] = [p[1], p[3]]


def p_ir_alloca_no_addrspace(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-addrspace-attachment : ir-instruction-attachment-list
    """
    p[0] = [0, p[1]]


def p_ir_loadstore_align(p):
    # type: (YaccProduction) -> None
    """
        ir-loadstore-attachment-list : COMMA ALIGN ir-literal-decimal ir-instruction-attachment-list
    """
    p[0] = [p[3], p[4]]


def p_ir_loadstore_noalign(p):
    # type: (YaccProduction) -> None
    """
        ir-loadstore-attachment-list : ir-instruction-attachment-list
    """
    p[0] = [None, p[1]]


def p_ir_inalloca_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-inalloca-opt : INALLOCA
                        | empty
    """


def p_ir_volatile_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-volatile-opt : VOLATILE
                        | empty
    """


def p_ir_inbounds_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-inbounds-opt : INBOUNDS
                        | empty
    """


def p_ir_access_list(p):
    # type: (YaccProduction) -> None
    """
        ir-access-list : COMMA ir-inrange-opt ir-value ir-access-list
    """
    p[0] = [[p[3]] + p[4][0], p[4][1]]


def p_ir_access_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-access-list : ir-instruction-attachment-list
    """
    p[0] = [[], p[1]]


def p_ir_inramnge_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-inrange-opt : INRANGE
                       | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction