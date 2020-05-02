from be_typing import TYPE_CHECKING


def p_ir_comdat(p):
    # type: (YaccProduction) -> None
    """
        ir-comdat : ID_COMDAT EQUAL COMDAT ir-comdat-kind
    """
    p[0] = []


def p_ir_comdat_kind(p):
    # type: (YaccProduction) -> None
    """
        ir-comdat-kind : ANY
                       | EXACTMATCH
                       | LARGEST
                       | NODUPLICATES
                       | SAMESIZE
    """
    p[0] = []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction