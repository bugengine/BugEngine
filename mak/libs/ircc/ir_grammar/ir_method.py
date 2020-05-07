"""
define [linkage] [PreemptionSpecifier] [visibility] [DLLStorageClass]
       [cconv] [ret attrs]
       <ResultType> @<FunctionName> ([argument list])
       [(unnamed_addr|local_unnamed_addr)] [AddrSpace] [fn Attrs]
       [section "name"] [comdat [($name)]] [align N] [gc] [prefix Constant]
       [prologue Constant] [personality Constant] (!name !N)* { ... }
"""

from ..ir_ast import IrReference, IrMethod, IrMethodBody, IrMethodParameter, IrMethodMetadataParameter
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
        ir-method-definition : DEFINE ir-method-prototype ir-metadata-list LBRACE ir-instruction-list RBRACE
    """
    method = p[2][1]
    p[0] = p[2]


def p_ir_method_prototype(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prototype : ir-method-header ir-parameter-attribute-list ir-return-type ID LPAREN LPAREN_MARK ir-parameter-list RPAREN ir-method-footer 
    """
    p[0] = (IrReference(p[4]), IrMethod(p[3], p[7], p[1]))


def p_ir_method_header(p):
    # type: (YaccProduction) -> None
    """
        ir-method-header : ir-metadata-list ir-linkage ir-preemption-specifier ir-visibility ir-dll-storage ir-calling-convention
    """
    p[0] = p[6]


def p_ir_method_footer(p):
    # type: (YaccProduction) -> None
    """
        ir-method-footer : ir-method-addr ir-addrspace-opt ir-method-attribute-list ir-method-section ir-method-comdat ir-method-align ir-method-gc ir-method-prefix ir-method-prologue ir-method-personality
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
                   | EXTERNAL
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
        ir-dll-storage : empty
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
    # TODO: error


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


def p_ir_method_addr(p):
    # type: (YaccProduction) -> None
    """
        ir-method-addr : empty
                       | UNNAMED_ADDR
                       | LOCAL_UNNAMED_ADDR
    """


def p_ir_denormal(p):
    # type: (YaccProduction) -> None
    """
        ir-denormal : IEEE
                    | PRESERVE_SIGN
                    | POSITIVE_ZERO
    """


def p_ir_method_section(p):
    # type: (YaccProduction) -> None
    """
        ir-method-section : empty
                          | SECTION LITERAL_STRING
    """


def p_ir_method_gc(p):
    # type: (YaccProduction) -> None
    """
        ir-method-gc : empty
                     | GC
    """


def p_ir_method_comdat(p):
    # type: (YaccProduction) -> None
    """
        ir-method-comdat : empty
                         | COMDAT
                         | COMDAT LPAREN LPAREN_MARK ID_COMDAT RPAREN
    """


def p_ir_method_align(p):
    # type: (YaccProduction) -> None
    """
        ir-method-align : empty
                        | ALIGN LITERAL_DECIMAL
    """


def p_ir_method_prefix(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prefix : empty
                         | PREFIX ir-constant
    """


def p_ir_method_prologue(p):
    # type: (YaccProduction) -> None
    """
        ir-method-prologue : empty
                           | PROLOGUE ir-constant
    """


def p_ir_method_personality(p):
    # type: (YaccProduction) -> None
    """
        ir-method-personality : empty
                              | PERSONALITY ir-constant
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction