from be_typing import TYPE_CHECKING


def p_ir_opcode_vector_extract(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : EXTRACTELEMENT ir-type ir-value COMMA ir-type ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_vector_insert(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : INSERTELEMENT ir-type ir-value COMMA ir-type ir-value COMMA ir-type ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_vector_shuffle(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SHUFFLEVECTOR ir-type ir-value COMMA ir-type ir-value COMMA ir-type ir-value ir-instruction-attachment-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction