from ..ir_ast import IrValueExpr, IrValueVoid, IrValueMetadata
from be_typing import TYPE_CHECKING


def p_ir_value(p):
    # type: (YaccProduction) -> None
    """
        ir-value : ir-type ir-parameter-attribute-list-opt ir-expr
    """
    p[0] = IrValueExpr(p[1], p[3])


def p_ir_value_metadata(p):
    # type: (YaccProduction) -> None
    """
        ir-value : METADATA ir-value
                 | METADATA ir-metadata-value
    """
    p[0] = IrValueMetadata(p[1])


def p_ir_value_void(p):
    # type: (YaccProduction) -> None
    """
        ir-value : VOID
    """
    p[0] = IrValueVoid()


def p_ir_value_list(p):
    # type: (YaccProduction) -> None
    """
        ir-value-list : ir-value COMMA ir-value-list
    """
    p[0] = [p[1]] + p[3]


def p_ir_value_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-value-list : ir-value
                      | empty
    """
    p[0] = [p[1]] if p[1] else []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction