from be_typing import TYPE_CHECKING


def p_ir_header(p):
    # type: (YaccProduction) -> None
    """
        ir-header : ir-header-decl ir-header
    """
    p[0] = []


def p_ir_header_end(p):
    # type: (YaccProduction) -> None
    """
        ir-header : empty
    """
    p[0] = []


def p_ir_header_module_name(p):
    # type: (YaccProduction) -> None
    """
        ir-header-decl : SOURCE_FILENAME EQUAL LITERAL_STRING
    """
    p[0] = None


def p_ir_header_data_layout(p):
    # type: (YaccProduction) -> None
    """
        ir-header-decl : TARGET DATALAYOUT EQUAL LITERAL_STRING
    """
    p[0] = None


def p_ir_header_triple(p):
    # type: (YaccProduction) -> None
    """
        ir-header-decl : TARGET TRIPLE EQUAL LITERAL_STRING
    """
    p[0] = None


if TYPE_CHECKING:
    from ply.yacc import YaccProduction