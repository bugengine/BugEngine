from be_typing import TYPE_CHECKING


def p_ir_opcode_alloca(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ALLOCA ir-inalloca-opt ir-type ir-alloca-attachment-list
    """


def p_ir_opcode_load(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : LOAD ir-volatile-opt ir-type COMMA ir-type ir-value ir-loadstore-attachment-list 
    """


def p_ir_opcode_store(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : STORE ir-volatile-opt ir-type ir-value COMMA ir-type ir-value ir-loadstore-attachment-list 
    """


def p_ir_opcode_getelementptr(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : GETELEMENTPTR ir-inbounds-opt ir-type ir-access-list 
    """


def p_ir_alloca_attachment_list(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-attachment-list : COMMA ir-constant ir-alloca-align-attachment
                                  | ir-alloca-align-attachment
        ir-alloca-align-attachment : COMMA ALIGN LITERAL_DECIMAL ir-alloca-addrspace-attachment
                                   | ir-alloca-addrspace-attachment
        ir-alloca-addrspace-attachment : COMMA ir-addrspace ir-instruction-attachment-list
                                       | ir-instruction-attachment-list
    """


def p_ir_loadstore_attachment_list(p):
    # type: (YaccProduction) -> None
    """
        ir-loadstore-attachment-list : COMMA ALIGN LITERAL_DECIMAL ir-instruction-attachment-list
                                     | ir-instruction-attachment-list
    """


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
        ir-access-list : COMMA ir-inrange-opt ir-type ir-value ir-access-list
                       | ir-instruction-attachment-list
    """


def p_ir_inramnge_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-inrange-opt : INRANGE
                       | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction