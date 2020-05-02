from be_typing import TYPE_CHECKING


def p_ir_opcode_alloca(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ALLOCA ir-alloca-in ir-type ir-alloca-count ir-alloca-align ir-alloca-addrspace
                  | ALLOCA ir-alloca-in ir-type ir-alloca-count ir-alloca-align
                  | ALLOCA ir-alloca-in ir-type ir-alloca-count ir-alloca-addrspace
                  | ALLOCA ir-alloca-in ir-type ir-alloca-count
                  | ALLOCA ir-alloca-in ir-type ir-alloca-align ir-alloca-addrspace
                  | ALLOCA ir-alloca-in ir-type ir-alloca-align
                  | ALLOCA ir-alloca-in ir-type ir-alloca-addrspace
                  | ALLOCA ir-alloca-in ir-type 
    """


def p_ir_alloca_in(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-in : INALLOCA
                     | empty
    """

def p_ir_alloca_count(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-count : COMMA ir-constant
    """

def p_ir_alloca_align(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-align : COMMA ALIGN LITERAL_DECIMAL
    """

def p_ir_alloca_addrspace(p):
    # type: (YaccProduction) -> None
    """
        ir-alloca-addrspace : COMMA ADDRSPACE LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction