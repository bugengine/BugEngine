from ..ir_ast import IrAttributeGroupDeclaration, IrAttributeGroupLink, IrAttributeGroupObject, IrAttribute, IrReference, IrExpressionConstant
from ply.lex import LexToken
from be_typing import TYPE_CHECKING


def p_ir_declaration_attributes(p):
    # type: (YaccProduction) -> None
    """
        ir-attributes : ATTRIBUTES ATTRIBUTE_GROUP EQUAL LBRACE ir-attribute-list-opt RBRACE
    """
    p[0] = (IrReference(p[2]), IrAttributeGroupDeclaration(p[5]))


def p_ir_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-attribute-list-opt : ir-method-attribute ir-method-attribute-list-opt
                              | ir-parameter-attribute ir-parameter-attribute-list-opt
                              | ir-any-attribute ir-attribute-list-opt
    """
    p[0] = IrAttributeGroupObject([p[1]] + p[2]._attributes)


def p_ir_method_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-method-attribute-list-opt : ir-method-attribute ir-method-attribute-list-opt
                                     | ir-any-attribute ir-method-attribute-list-opt
    """
    p[0] = IrAttributeGroupObject([p[1]] + p[2]._attributes)


def p_ir_parameter_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-attribute-list-opt : ir-parameter-attribute ir-parameter-attribute-list-opt
                                        | ir-any-attribute ir-parameter-attribute-list-opt
    """
    p[0] = IrAttributeGroupObject([p[1]] + p[2]._attributes)


def p_ir_attribute_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-attribute-list-opt : empty
        ir-method-attribute-list-opt : empty
        ir-parameter-attribute-list-opt : empty
    """
    p[0] = IrAttributeGroupObject([])


def p_ir_attribute_parameter(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-attribute : ZEROEXT
                               | SIGNEXT
                               | INREG
                               | BYVAL
                               | PREALLOCATED
                               | INALLOCA
                               | SRET
                               | NOALIAS
                               | NOCAPTURE
                               | NEST
                               | RETURNED
                               | NONNULL
                               | SWIFTSELF
                               | SWIFTERROR
                               | IMMARG
    """
    p[0] = IrAttribute(p[1])


def p_ir_attribute_parameter_value(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-attribute : BYVAL LPAREN LPAREN_MARK ir-type RPAREN
                               | PREALLOCATED LPAREN LPAREN_MARK ir-literal-decimal RPAREN
                               | ALIGN ir-literal-decimal
                               | DEREFERENCEABLE LPAREN LPAREN_MARK ir-literal-decimal RPAREN
                               | DEREFERENCEABLE_OR_NULL LPAREN LPAREN_MARK ir-literal-decimal RPAREN
    """
    if len(p) > 3:
        p[0] = IrAttribute(p[1], [p[4]])
    else:
        p[0] = IrAttribute(p[1], [p[2]])


def p_ir_attribute_method(p):
    # type: (YaccProduction) -> None
    """
        ir-method-attribute : ALIGNSTACK LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
                            | ALLOCSIZE LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
                            | ALLOCSIZE LPAREN LPAREN_MARK LITERAL_DECIMAL COMMA LITERAL_DECIMAL RPAREN
                            | ALWAYSINLINE
                            | BUILTIN
                            | COLD
                            | CONVERGENT
                            | INACCESSIBLEMEMONLY
                            | INACCESSIBLEMEM_OR_ARGMEMONLY
                            | INLINEHINT
                            | JUMPTABLE
                            | MINSIZE
                            | NAKED
                            | NO_INLINE_LINE_TABLES
                            | NO_JUMP_TABLES
                            | NOBUILTIN
                            | NODUPLICATE
                            | NOIMPLICITFLOAT
                            | NOINLINE
                            | NONLAZYBIND
                            | NOREDZONE
                            | INDIRECT_TLS_SEG_REFS
                            | NORETURN
                            | NORECURSE
                            | WILLRETURN
                            | NOSYNC
                            | NOUNWIND
                            | NULL_POINTER_IS_VALID
                            | OPTFORFUZZING
                            | OPTNONE
                            | OPTSIZE
                            | PATCHABLE_FUNCTION LITERAL_STRING
                            | PROBE_STACK LITERAL_STRING
                            | STACK_PROBE_SIZE LITERAL_DECIMAL
                            | NO_STACK_ARG_PROBE
                            | ARGMEMONLY
                            | RETURNS_TWICE
                            | SAFESTACK
                            | SANITIZE_ADDRESS
                            | SANITIZE_MEMORY
                            | SANITIZE_MEMTAG
                            | SPECULATIVE_LOAD_HARDENING
                            | SPECULATABLE
                            | SSP
                            | SSPREQ
                            | SSPSTRONG
                            | STRICTFP
                            | DENORMAL_FP_MATH ir-denormal COMMA ir-denormal
                            | DENORMAL_FP_MATH_F32
                            | THUNK
                            | UWTABLE
                            | NOCF_CHECK
                            | SHADOWCALLSTACK
                            | POISON
                            | MUSTPROGRESS
    """
    p[0] = IrAttribute(p[1])


def p_ir_attribute_any(p):
    # type: (YaccProduction) -> None
    """
        ir-any-attribute : LITERAL_STRING EQUAL LITERAL_STRING
                         | NOFREE
                         | READNONE
                         | READONLY
                         | WRITEONLY
    """
    p[0] = IrAttribute(p[1])


def p_ir_attribute_group_ref(p):
    # type: (YaccProduction) -> None
    """
        ir-any-attribute : ATTRIBUTE_GROUP
    """
    p[0] = IrAttributeGroupLink(IrReference(p[1]))


def p_ir_literal_decimal(p):
    # type: (YaccProduction) -> None
    """
        ir-literal-decimal : LITERAL_DECIMAL
    """
    assert isinstance(p.slice[1], LexToken)
    p[0] = IrExpressionConstant(p.slice[1].parsed_value)


if TYPE_CHECKING:
    from ply.yacc import YaccProduction