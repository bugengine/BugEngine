"""
@<GlobalVarName> = [Linkage] [PreemptionSpecifier] [Visibility]
                   [DLLStorageClass] [ThreadLocal]
                   [(unnamed_addr|local_unnamed_addr)] [AddrSpace]
                   [ExternallyInitialized]
                   <global | constant> <Type> [<InitializerConstant>]
                   [, section "name"] [, comdat [($name)]]
                   [, align <Alignment>] (, !name !N)*
"""

from ..ir_ast import IrReference, IrVariable, IrVariableDeclaration
from be_typing import TYPE_CHECKING


def p_ir_variable(p):
    # type: (YaccProduction) -> None
    """
        ir-variable : ID EQUAL ir-linkage ir-preemption-specifier ir-visibility ir-dll-storage-opt ir-method-addr-opt ir-addrspace-opt ir-variable-global-constant-opt ir-type ir-expr ir-variable-footer
                    | ID EQUAL EXTERNAL   ir-preemption-specifier ir-visibility ir-dll-storage-opt ir-method-addr-opt ir-addrspace-opt ir-variable-global-constant-opt ir-type empty ir-variable-footer
    """
    p[0] = (IrReference(p[1]), IrVariableDeclaration(IrVariable(p[1], p[10], p[11], p[8], p[12])))


def p_ir_variable_footer(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-footer : COMMA SECTION LITERAL_STRING ir-variable-comdat
    """
    p[0] = p[4]


def p_ir_variable_footer_empty(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-footer : ir-variable-comdat
    """
    p[0] = p[1]


def p_ir_variable_comdat(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-comdat : COMMA COMDAT ID_COMDAT ir-variable-align
    """
    p[0] = p[4]


def p_ir_variable_comdat_empty(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-comdat : ir-variable-align
    """
    p[0] = p[1]


def p_ir_variable_align(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-align : COMMA ALIGN LITERAL_DECIMAL ir-variable-metadata
    """
    p[0] = p[4]


def p_ir_variable_align_empty(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-align : ir-variable-metadata
    """
    p[0] = p[1]


def p_ir_variable_metadata(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-metadata : COMMA METADATA_NAME METADATA_REF ir-variable-metadata
    """
    p[0] = p[4]


def p_ir_variable_metadata_empty(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-metadata : empty
    """
    p[0] = []


def p_ir_variable_global_constant_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-variable-global-constant-opt : GLOBAL
                                        | CONSTANT
                                        | empty
    """
    p[0] = p[1]


if TYPE_CHECKING:
    from ply.yacc import YaccProduction