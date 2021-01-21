from ..ir_ast import IrTypeMetadata, IrMetadataDeclaration, IrMetadataString, IrMetadataInteger, IrMetadataLink, IrMetadataNode, IrSpecializedMetadata, IrMetadataNull, IrMetadataFlagList, IrReference
from be_typing import TYPE_CHECKING


def p_ir_metadata_list_opt(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-list-opt : METADATA_NAME METADATA_REF ir-metadata-list-opt
    """
    p[0] = [(IrMetadataLink(p[1]), IrMetadataLink(p[2]))] + p[3]


def p_ir_metadata_list_opt_empty(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-list-opt :
    """
    p[0] = []


def p_ir_metadata(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata : METADATA_NAME EQUAL ir-metadata-distinct ir-metadata-value
                    | METADATA_REF EQUAL ir-metadata-distinct ir-metadata-value
    """
    p[0] = (IrReference(p[1]), IrMetadataDeclaration(p[4]))


def p_ir_metadata_distinct(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-distinct : DISTINCT
                             | empty
    """
    p[0] = None


def p_ir_metadata_value(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-value : ir-metadata-string
                          | ir-metadata-node
                          | ir-metadata-debug-node
                          | ir-metadata-ref
    """
    p[0] = p[1]


def p_ir_metadata_null(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-value : NULL
    """
    p[0] = IrMetadataNull()


def p_ir_metadata_ref(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-ref : METADATA_REF
    """
    p[0] = IrMetadataLink(p[1])


def p_ir_metadata_string(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-string : METADATA_MARK LITERAL_STRING
    """
    p[0] = IrMetadataString(p[2])


def p_ir_metadata_node(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-node : METADATA_MARK LBRACE ir-metadata-param-list RBRACE
    """
    p[0] = IrMetadataNode(p[3])


def p_ir_metadata_param_list(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-param-list : ir-metadata-param COMMA ir-metadata-param-list
    """
    p[0] = [p[1]] + p[3]


def p_ir_metadata_param_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-param-list : ir-metadata-param
                               | empty
    """
    p[0] = [p[1]] if p[1] is not None else []


def p_ir_metadata_param(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-param : ir-value
                          | ir-metadata-value
    """
    p[0] = p[1]


def p_lex_disable_keywords(p):
    # type: (YaccProduction) -> None
    """
        lex-disable-keywords : empty
    """
    p.lexer.disable_keywords()


def p_lex_enable_keywords(p):
    # type: (YaccProduction) -> None
    """
        lex-enable-keywords : empty
    """
    p.lexer.enable_keywords()


def p_ir_metadata_debug_node(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-node : METADATA_NAME LPAREN lex-disable-keywords LPAREN_MARK ir-metadata-debug-attribute-list-opt RPAREN lex-enable-keywords
    """
    p[0] = IrSpecializedMetadata(p[1][1:], p[5])


def p_ir_metadata_debug_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute-list-opt : ID_LABEL lex-enable-keywords COLON ir-metadata-debug-attribute lex-disable-keywords COMMA ir-metadata-debug-attribute-list-opt
                                             | ID_LABEL lex-enable-keywords COLON ir-metadata-debug-attribute lex-disable-keywords
    """
    if p[4]:
        p[0] = [(p[1], p[4])]
    else:
        p[0] = []
    if len(p) > 6:
        p[0] += p[7]


def p_ir_metadata_debug_attribute_list_unnamed(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute-list-opt : ir-metadata-debug-attribute COMMA ir-metadata-debug-attribute-list-opt
                                             | ir-metadata-debug-attribute
    """
    p[0] = [(None, p[1])]
    if len(p) > 2:
        p[0] += p[3]


def p_ir_metadata_debug_attribute_list_end(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute-list-opt : empty
    """
    p[0] = []


def p_ir_metadata_debug_attribute(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : ir-metadata-value
                                    | ir-value
                                    | ir-metadata-debug-flag-combination
    """
    p[0] = p[1]


def p_ir_metadata_debug_attribute_string(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : LITERAL_STRING
    """
    p[0] = IrMetadataString(getattr(p.slice[1], 'parsed_value'))


def p_ir_metadata_debug_attribute_integer(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : LITERAL_DECIMAL
    """
    p[0] = IrMetadataInteger(getattr(p.slice[1], 'parsed_value'))


def p_ir_metadata_debug_attribute_bool(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : TRUE
                                    | FALSE
    """


def p_ir_metadata_debug_attribute_none(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : NONE
    """
    p[0] = None    #IrMetadataInteger(getattr(p.slice[1], 'parsed_value'))


def p_ir_metadata_debug_attribute_error(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-attribute : error
    """
    p[0] = None


def p_ir_metadata_debug_flag_combination(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-flag-combination : ir-metadata-debug-flag PIPE ir-metadata-debug-flag-combination
                                           | ir-metadata-debug-flag
    """
    if len(p) > 2:
        p[0] = p[3]
        p[0].add_flag(p[1])
    else:
        p[0] = IrMetadataFlagList(p[1])


def p_ir_metadata_debug_flag(p):
    # type: (YaccProduction) -> None
    """
        ir-metadata-debug-flag : ID_LABEL
    """
    p[0] = p[1]


if TYPE_CHECKING:
    from ply.yacc import YaccProduction