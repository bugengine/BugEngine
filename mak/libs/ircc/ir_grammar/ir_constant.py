from be_typing import TYPE_CHECKING


def p_ir_constant(p):
    # type: (YaccProduction) -> None
    """
        ir-constant : ir-type ir-value
                    | METADATA ir-type ir-value
                    | METADATA ir-metadata-value
                    | VOID
    """
    p[0] = []


def p_ir_value(p):
    # type: (YaccProduction) -> None
    """
        ir-value : LITERAL_DECIMAL
                 | UNDEF
                 | NULL
                 | ID
    """
    p[0] = []


def p_ir_value_aggregated(p):
    # type: (YaccProduction) -> None
    """
        ir-value : LBRACKET ir-value-list RBRACKET
    """
    p[0] = []


def p_ir_value_list(p):
    # type: (YaccProduction) -> None
    """
        ir-value-list : ir-constant COMMA ir-value-list
                      | ir-constant
                      | empty
    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction