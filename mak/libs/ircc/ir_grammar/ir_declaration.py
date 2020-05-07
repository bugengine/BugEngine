from be_typing import TYPE_CHECKING


def p_ir_declaration_list(p):
    # type: (YaccProduction) -> None
    """
        ir-declaration-list : ir-declaration-list ir-declaration
    """
    p[0] = p[1] + ([p[2]] if p[2] is not None else [])


def p_ir_declaration_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-declaration-list : empty
    """
    p[0] = []


def p_ir_declaration(p):
    # type: (YaccProduction) -> None
    """
        ir-declaration : ir-typedecl
                       | ir-comdat
                       | ir-method
                       | ir-attributes
                       | ir-metadata
    """
    p[0] = p[1]


if TYPE_CHECKING:
    from ply.yacc import YaccProduction