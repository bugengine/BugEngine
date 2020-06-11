from ...ir_ast.instructions import IrInstExtractElement, IrInstInsertElement, IrInstShuffleVector
from be_typing import TYPE_CHECKING


def p_ir_opcode_vector_extract(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment EXTRACTELEMENT ir-value COMMA ir-value ir-instruction-attachment-list
    """
    p[0] = IrInstExtractElement(p[1], p[3], p[5], p[6])


def p_ir_opcode_vector_insert(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment INSERTELEMENT ir-value COMMA ir-value COMMA ir-value ir-instruction-attachment-list
    """
    p[0] = IrInstInsertElement(p[1], p[3], p[5], p[7], p[8])


def p_ir_opcode_vector_shuffle(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment SHUFFLEVECTOR ir-value COMMA ir-value COMMA ir-value ir-instruction-attachment-list
    """
    p[0] = IrInstShuffleVector(p[1], p[3], p[5], p[7], p[8])


if TYPE_CHECKING:
    from ply.yacc import YaccProduction