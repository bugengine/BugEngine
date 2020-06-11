from ...ir_ast.instructions import IrInstExtractValue, IrInstInsertValue
from be_typing import TYPE_CHECKING


def p_ir_opcode_aggregate_extract(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment EXTRACTVALUE ir-value ir-aggregate-index-list
    """
    p[0] = IrInstExtractValue(p[1], p[3], p[4][0], p[4][1])


def p_ir_opcode_aggregate_insert(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : ir-instruction-assignment INSERTVALUE ir-value COMMA ir-value ir-aggregate-index-list
    """
    p[0] = IrInstInsertValue(p[1], p[3], p[5], p[6][0], p[6][1])


def p_ir_aggregate_index_list(p):
    # type: (YaccProduction) -> None
    """                                                        
        ir-aggregate-index-list : COMMA ir-literal-decimal ir-aggregate-index-list
    """
    p[0] = ([p[2]] + p[3][0], p[3][1])


def p_ir_aggregate_index_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-aggregate-index-list : COMMA ir-literal-decimal ir-instruction-attachment-list
    """
    p[0] = ([p[2]], p[3])


if TYPE_CHECKING:
    from ply.yacc import YaccProduction