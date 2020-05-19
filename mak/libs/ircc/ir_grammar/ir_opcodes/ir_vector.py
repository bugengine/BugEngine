from be_typing import TYPE_CHECKING


def p_ir_opcode_vector_extract(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : EXTRACTELEMENT ir-value COMMA ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_vector_insert(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : INSERTELEMENT ir-value COMMA ir-value COMMA ir-value ir-instruction-attachment-list
    """


def p_ir_opcode_vector_shuffle(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : SHUFFLEVECTOR ir-value COMMA ir-value COMMA ir-value ir-instruction-attachment-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction