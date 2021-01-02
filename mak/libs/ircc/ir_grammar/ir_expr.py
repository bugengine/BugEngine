from ..ir_ast import IrExpressionCast, IrExpressionConstant, IrExpressionReference, IrExpressionArray, IrExpressionAggregate, IrExpressionUndef, IrReference, IrExpressionZero
from be_typing import TYPE_CHECKING


def p_ir_expr(p):
    # type: (YaccProduction) -> None
    """
        ir-expr : ir-expr-constant
                | ir-expr-zeroinitializer
                | ir-expr-undef
                | ir-expr-reference
                | ir-expr-cast
                | ir-expr-aggregate
                | ir-expr-array
    """
    p[0] = p[1]


def p_ir_expr_constant_decimal(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-constant : LITERAL_DECIMAL
    """
    p[0] = IrExpressionConstant(getattr(p.slice[1], 'parsed_value'))


def p_ir_expr_constant_null(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-constant : NULL
    """
    p[0] = IrExpressionConstant(None)


def p_ir_expr_constant_bool(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-constant : TRUE
                         | FALSE
    """
    p[0] = IrExpressionConstant(p[1] == 'true')


def p_ir_expr_zeroinitializer(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-zeroinitializer : ZEROINITIALIZER
    """
    p[0] = IrExpressionZero()


def p_ir_expr_undef(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-undef : UNDEF
    """
    p[0] = IrExpressionUndef()


def p_ir_expr_reference(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-reference : ID
    """
    p[0] = IrExpressionReference(IrReference(p[1]))


def p_ir_expr_cast(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-cast : ir-cast-op LPAREN LPAREN_MARK ir-value TO ir-type RPAREN
    """
    p[0] = IrExpressionCast(p[6], p[4], p[1])


def p_ir_expr_array(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-array : LBRACKET ir-value-list RBRACKET
    """
    p[0] = IrExpressionArray(p[2])


def p_ir_expr_aggregate(p):
    # type: (YaccProduction) -> None
    """
        ir-expr-aggregate : LBRACE ir-value-list RBRACE
    """
    p[0] = IrExpressionAggregate(p[2])


if TYPE_CHECKING:
    from ply.yacc import YaccProduction