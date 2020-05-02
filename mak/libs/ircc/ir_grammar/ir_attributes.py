from be_typing import TYPE_CHECKING


def p_ir_declaration_attributes(p):
    # type: (YaccProduction) -> None
    """
        ir-attributes : ATTRIBUTES ATTRIBUTE_GROUP EQUAL LBRACE ir-attribute-list RBRACE
    """
    p[0] = None


def p_ir_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-attribute-list : ir-method-attribute ir-method-attribute-list
                          | ir-parameter-attribute ir-parameter-attribute-list
                          | ir-any-attribute ir-parameter-attribute-list
                          | empty
    """
    p[0] = None


def p_ir_method_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-method-attribute-list : ir-method-attribute ir-method-attribute-list
                                 | ir-any-attribute ir-method-attribute-list
                                 | empty
    """
    p[0] = None


def p_ir_parameter_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-attribute-list : ir-parameter-attribute ir-parameter-attribute-list
                                    | ir-any-attribute ir-parameter-attribute-list
                                    | empty
    """
    p[0] = None


def p_ir_attribute_parameter(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-attribute : ZEROEXT
                               | SIGNEXT
                               | INREG
                               | BYVAL
                               | BYVAL LPAREN LPAREN_MARK ir-type RPAREN
                               | PREALLOCATED
                               | PREALLOCATED LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
                               | INALLOCA
                               | SRET
                               | ALIGN LITERAL_DECIMAL
                               | NOALIAS
                               | NOCAPTURE
                               | NEST
                               | RETURNED
                               | NONNULL
                               | DEREFERENCEABLE LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
                               | DEREFERENCEABLE_OR_NULL LPAREN LPAREN_MARK LITERAL_DECIMAL RPAREN
                               | SWIFTSELF
                               | SWIFTERROR
                               | IMMARG
    """


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
                            | ATTRIBUTE_GROUP
    """


def p_ir_attribute_any(p):
    # type: (YaccProduction) -> None
    """
        ir-any-attribute : LITERAL_STRING EQUAL LITERAL_STRING
                         | NOFREE
                         | READNONE
                         | READONLY
                         | WRITEONLY
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction