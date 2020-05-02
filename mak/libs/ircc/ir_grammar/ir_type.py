from be_typing import TYPE_CHECKING


def p_ir_typedecl(p):
    # type: (YaccProduction) -> None
    """
        ir-typedecl : ID EQUAL TYPE ir-type
    """
    p[0] = []


def p_ir_type_struct(p):
    # type: (YaccProduction) -> None
    """
        ir-type : LBRACE ir-type-list RBRACE
    """
    p[0] = []


def p_ir_type_struct_compact(p):
    # type: (YaccProduction) -> None
    """
        ir-type : LANGLE LBRACE ir-type-list RBRACE RANGLE
    """
    p[0] = []


def p_ir_typedef_opaque(p):
    # type: (YaccProduction) -> None
    """
        ir-type : OPAQUE
    """
    p[0] = []


def p_ir_typelist(p):
    # type: (YaccProduction) -> None
    """
        ir-type-list : ir-type COMMA ir-type-list
    """
    p[0] = []


def p_ir_typelist_end(p):
    # type: (YaccProduction) -> None
    """
        ir-type-list : ir-type
                     | empty
    """
    p[0] = []


def p_ir_type(p):
    # type: (YaccProduction) -> None
    """
        ir-type : ir-type-id
                | ir-type-scalar
                | ir-type-vector
                | ir-type-ptr
                | ir-type-array
    """
    p[0] = p[1]


def p_ir_type_id(p):
    # type: (YaccProduction) -> None
    """
        ir-type-id : ID
    """
    p[0] = []


def p_ir_type_scalar(p):
    # type: (YaccProduction) -> None
    """
        ir-type-scalar : I1
                       | I8
                       | I16
                       | I32
                       | I64
                       | FLOAT
                       | HALF
                       | DOUBLE
    """
    p[0] = []


def p_ir_type_builtin_vector(p):
    # type: (YaccProduction) -> None
    """
        ir-type-vector : LANGLE LITERAL_DECIMAL X ir-type-scalar RANGLE
    """


def p_ir_type_ptr(p):
    # type: (YaccProduction) -> None
    """
        ir-type-ptr : ir-type ir-addrspace STAR
                    | VOID ir-addrspace STAR
    """


def p_ir_addrspace(p):
    # type: (YaccProduction) -> None
    """
        ir-addrspace : ADDRSPACE LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
    """


def p_ir_type_addrspace_none(p):
    # type: (YaccProduction) -> None
    """
        ir-addrspace : empty
    """


def p_ir_type_array(p):
    # type: (YaccProduction) -> None
    """
        ir-type-array : LBRACKET LITERAL_DECIMAL X ir-type RBRACKET
    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction