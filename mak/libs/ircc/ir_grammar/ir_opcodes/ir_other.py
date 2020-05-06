from be_typing import TYPE_CHECKING


def p_ir_opcode_call(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-tail-type CALL ir-calling-convention ir-parameter-attribute-list ir-return-type ID LPAREN LPAREN_MARK ir-argument-list RPAREN ir-attribute-list ir-instruction-attachment-list
    """


def p_ir_opcode_cmp(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ICMP ir-cmp-op ir-type ir-value COMMA ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_fcmp(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : FCMP ir-fcmp-op ir-type ir-value COMMA ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_phi(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : PHI ir-type ir-phi-attachment-list
    """


def p_ir_opcode_select(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SELECT ir-type ir-value COMMA ir-type ir-value COMMA ir-type ir-value ir-instruction-attachment-list
    """


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


def p_ir_phi_attachment_list(p):
    # type: (YaccProduction) -> None
    """
        ir-phi-attachment-list : LBRACKET ir-value COMMA ir-value RBRACKET COMMA ir-phi-attachment-list
                               | LBRACKET ir-value COMMA ir-value RBRACKET ir-instruction-attachment-list
    """


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


def p_ir_argument_list(p):
    # type: (YaccProduction) -> None
    """
        ir-argument-list : ir-constant COMMA ir-argument-list
                         | ir-constant
                         | empty
    """
    p[0] = []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction