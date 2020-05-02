from be_typing import TYPE_CHECKING


def p_ir_opcode_call(p):
    # type: (YaccProduction) -> None
    """
        ir-opcode : CALL ir-calling-convention ir-return-type ID LPAREN LPAREN_MARK ir-argument-list RPAREN ir-attribute-list
    """


def p_ir_argument_list(p):
    # type: (YaccProduction) -> None
    """
        ir-argument-list : ir-constant COMMA ir-argument-list
                         | ir-constant
                         | empty
    """
    p[0] = []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction