from ..ir_ast import IrTypeBuiltin, IrTypePtr, IrTypeVoid, IrTypeArray, IrTypeVector, IrTypeStruct, IrTypeReference, IrTypeOpaque, IrTypeMethod, IrReference, IrTypeDeclaration
from ply.lex import LexToken
from be_typing import TYPE_CHECKING


def p_ir_typedecl(p):
    # type: (YaccProduction) -> None
    """
        ir-typedecl : ID EQUAL TYPE ir-type
    """
    p[0] = (IrReference(p[1]), IrTypeDeclaration(p[4]))


def p_ir_typelist(p):
    # type: (YaccProduction) -> None
    """
        ir-type-list : ir-type COMMA ir-type-list
    """
    p[0] = [p[1]] + p[3]


def p_ir_typelist_end(p):
    # type: (YaccProduction) -> None
    """
        ir-type-list : ir-type
                     | empty
    """
    p[0] = [p[1]] if p[1] is not None else []


def p_ir_type(p):
    # type: (YaccProduction) -> None
    """
        ir-type : ir-type-id
                | ir-type-scalar
                | ir-type-struct
                | ir-type-vector
                | ir-type-ptr
                | ir-type-array
                | ir-type-opaque
                | ir-type-method
    """
    p[0] = p[1]


def p_ir_type_id(p):
    # type: (YaccProduction) -> None
    """
        ir-type-id : ID
    """
    p[0] = IrTypeReference(IrReference(p[1]))


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
    p[0] = IrTypeBuiltin(p[1])


def p_ir_type_struct(p):
    # type: (YaccProduction) -> None
    """
        ir-type-struct : LBRACE ir-type-list RBRACE
    """
    p[0] = IrTypeStruct(p[2], False)


def p_ir_type_struct_compact(p):
    # type: (YaccProduction) -> None
    """
        ir-type-struct : LANGLE LBRACE ir-type-list RBRACE RANGLE
    """
    p[0] = IrTypeStruct(p[3], True)


def p_ir_type_builtin_vector(p):
    # type: (YaccProduction) -> None
    """
        ir-type-vector : LANGLE LITERAL_DECIMAL X ir-type-scalar RANGLE
                       | LANGLE LITERAL_DECIMAL X ir-type-ptr RANGLE
    """
    assert isinstance(p.slice[2], LexToken)
    assert isinstance(p.slice[2].parsed_value, int)
    p[0] = IrTypeVector(p[4], p.slice[2].parsed_value)


def p_ir_type_ptr(p):
    # type: (YaccProduction) -> None
    """
        ir-type-ptr : ir-type ir-addrspace-opt STAR
    """
    p[0] = IrTypePtr(p[1], p[2])


def p_ir_type_void_ptr(p):
    # type: (YaccProduction) -> None
    """
        ir-type-ptr : VOID ir-addrspace-opt STAR
    """
    p[0] = IrTypePtr(IrTypeVoid(), p[2])


def p_ir_addrspace_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-addrspace-opt : ir-addrspace
                         | empty
    """
    p[0] = p[1] if p[1] is not None else 0


def p_ir_addrspace(p):
    # type: (YaccProduction) -> None
    """
        ir-addrspace : ADDRSPACE LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
    """
    assert isinstance(p.slice[4], LexToken)
    assert isinstance(p.slice[4].parsed_value, int)
    p[0] = p.slice[4].parsed_value


def p_ir_type_array(p):
    # type: (YaccProduction) -> None
    """
        ir-type-array : LBRACKET LITERAL_DECIMAL X ir-type RBRACKET
    """
    assert isinstance(p.slice[2], LexToken)
    assert isinstance(p.slice[2].parsed_value, int)
    p[0] = IrTypeArray(p[4], p.slice[2].parsed_value)


def p_ir_typedef_opaque(p):
    # type: (YaccProduction) -> None
    """
        ir-type-opaque : OPAQUE
    """
    p[0] = IrTypeOpaque()


def p_ir_type_method(p):
    # type: (YaccProduction) -> None
    """
        ir-type-method : ir-return-type LPAREN LPAREN_MARK ir-type-list RPAREN
    """
    p[0] = IrTypeMethod(p[1], p[4])


if TYPE_CHECKING:
    from ply.yacc import YaccProduction