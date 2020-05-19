from be_typing import TYPE_CHECKING


def p_ir_opcode_aggregate_extract(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : EXTRACTVALUE ir-value ir-aggregate-index-list
    """


def p_ir_opcode_aggregate_insert(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : INSERTVALUE ir-value COMMA ir-value ir-aggregate-index-list
    """


def p_ir_aggregate_index_list(p):
    # type: (YaccProduction) -> None
    """
        ir-aggregate-index-list : COMMA LITERAL_DECIMAL ir-aggregate-index-list
                                | COMMA LITERAL_DECIMAL ir-instruction-attachment-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction