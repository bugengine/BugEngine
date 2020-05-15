"""
define [linkage] [PreemptionSpecifier] [visibility] [DLLStorageClass]
       [cconv] [ret attrs]
       <ResultType> @<FunctionName> ([argument list])
       [(unnamed_addr|local_unnamed_addr)] [AddrSpace] [fn Attrs]
       [section "name"] [comdat [($name)]] [align N] [gc] [prefix Constant]
       [prologue Constant] [personality Constant] (!name !N)* { ... }
"""

from ..ir_ast import IrReference, IrMethodObject, IrMethodBody, IrMethodParameter, IrMethodMetadataParameter, IrMethodDeclaration
from be_typing import TYPE_CHECKING


def p_ir_method(p):
    # type: (YaccProduction) -> None
    """
        ir-method : ir-method-declaration
                  | ir-method-definition
    """
    p[0] = p[1]


def p_ir_method_declaration(p):
    # type: (YaccProduction) -> None
    """
        ir-method-declaration : DECLARE ir-method-prototype
    """
    p[0] = p[2]


def p_ir_method_definition(p):
    # type: (YaccProduction) -> None
    """
        ir-method-definition : DEFINE ir-method-prototype ir-metadata-list-opt LBRACE ir-instruction-list RBRACE
    """
    method = p[2][1]
    p[0] = p[2]


def p_ir_method_prototype(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prototype : ir-method-header ir-parameter-attribute-list ir-return-type ID LPAREN LPAREN_MARK ir-parameter-list RPAREN ir-method-footer
    """
    p[0] = (IrReference(p[4]), IrMethodDeclaration(IrMethodObject(p[3], p[7], p[1])))


def p_ir_method_header(p):
    # type: (YaccProduction) -> None
    """
        ir-method-header : ir-metadata-list-opt ir-linkage ir-preemption-specifier ir-visibility ir-dll-storage-opt ir-calling-convention
                         | ir-metadata-list-opt EXTERNAL   ir-preemption-specifier ir-visibility ir-dll-storage-opt ir-calling-convention
    """
    p[0] = p[6]


def p_ir_method_footer(p):
    # type: (YaccProduction) -> None
    """
        ir-method-footer : ir-method-addr-opt ir-addrspace-opt ir-method-attribute-list ir-method-section-opt ir-method-comdat-opt ir-method-align-opt ir-method-gc-opt ir-method-prefix-opt ir-method-prologue-opt ir-method-personality
    """


def p_ir_return_type(p):
    # type: (YaccProduction) -> None
    """
        ir-return-type : ir-type
    """
    p[0] = p[1]


def p_ir_return_type_void(p):
    # type: (YaccProduction) -> None
    """
        ir-return-type : VOID
    """
    p[0] = None


def p_ir_linkage(p):
    # type: (YaccProduction) -> None
    """
        ir-linkage : empty
                   | PRIVATE
                   | INTERNAL
                   | AVAILABLE_EXTERNALLY
                   | COMMON
    """


def p_ir_linkage_disallowed(p):
    # type: (YaccProduction) -> None
    """
        ir-linkage : LINKER_PRIVATE
                   | LINKER_PRIVATE_WEAK
                   | LINKONCE
                   | WEAK
                   | APPENDING
                   | EXTERN_WEAK
                   | LINKONCE_ODR
                   | WEAK_ODR
    """


def p_ir_preemption_specifier(p):
    # type: (YaccProduction) -> None
    """
        ir-preemption-specifier : empty
                                | DSO_LOCAL
                                | DSO_PREEMPTABLE
    """


def p_ir_visibility(p):
    # type: (YaccProduction) -> None
    """
        ir-visibility : empty
                      | DEFAULT
                      | HIDDEN
                      | PROTECTED
    """


def p_ir_dll_storage(p):
    # type: (YaccProduction) -> None
    """
        ir-dll-storage-opt : empty
                           | DLLIMPORT
                           | DLLEXPORT
    """


def p_ir_calling_convention(p):
    # type: (YaccProduction) -> None
    """
        ir-calling-convention : empty
                              | SPIR_KERNEL
                              | SPIR_FUNC
    """
    p[0] = p[1]


def p_ir_calling_convention_disallowed(p):
    # type: (YaccProduction) -> None
    """
        ir-calling-convention : CCC
                              | FASTCC
                              | COLDCC
                              | WEBKIT_JSCC
                              | ANYREGCC
                              | PRESERVE_MOSTCC
                              | PRESERVE_ALLCC
                              | CXX_FAST_TLSCC
                              | SWIFTCC
                              | TAILCC
                              | CFGUARD_CHECKCC
                              | CC LITERAL_DECIMAL
    """
    p.lexer.logger.C0010(p.position(1), p[1])


def p_ir_parameter_list(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-list : ir-parameter COMMA ir-parameter-list
    """
    p[0] = [p[1]] + p[3]


def p_ir_parameter_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter-list : ir-parameter
                          | empty
    """
    p[0] = [p[1]] if p[1] is not None else []


def p_ir_parameter(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter : ir-type ir-parameter-attribute-list ID
                     | ir-type ir-parameter-attribute-list empty
    """
    p[0] = IrMethodParameter(p[1], p[3], p[2])


def p_ir_parameter_metadata(p):
    # type: (YaccProduction) -> None
    """
        ir-parameter : METADATA ir-parameter-attribute-list ID
                     | METADATA ir-parameter-attribute-list empty
    """
    p[0] = IrMethodMetadataParameter(p[2], p[3], [])


def p_ir_method_addr_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-addr-opt : empty
                           | UNNAMED_ADDR
                           | LOCAL_UNNAMED_ADDR
    """
    # unused
    p[0] = None


def p_ir_denormal(p):
    # type: (YaccProduction) -> None
    """
        ir-denormal : IEEE
                    | PRESERVE_SIGN
                    | POSITIVE_ZERO
    """
    # unused
    p[0] = None


def p_ir_method_section_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-section-opt : empty
                              | SECTION LITERAL_STRING
    """
    # unused
    p[0] = None


def p_ir_method_gc_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-gc-opt : empty
                         | GC
    """
    # unused
    p[0] = None


def p_ir_method_comdat_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-comdat-opt : empty
                             | COMDAT
                             | COMDAT LPAREN LPAREN_MARK ID_COMDAT RPAREN
    """
    # unused
    p[0] = None


def p_ir_method_align_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-align-opt : empty
                            | ALIGN LITERAL_DECIMAL
    """
    # unused
    p[0] = None


def p_ir_method_prefix_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prefix-opt : empty
                             | PREFIX ir-constant
    """
    # unused
    p[0] = None


def p_ir_method_prologue_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prologue-opt : empty
                               | PROLOGUE ir-constant
    """
    # unused
    p[0] = None


def p_ir_method_personality(p):
    # type: (YaccProduction) -> None
    """
        ir-method-personality : empty
                              | PERSONALITY ir-constant
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction