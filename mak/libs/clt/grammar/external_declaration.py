from ..cl_ast import specifiers


def p_declaration_specifier(p):
    """
        declaration_specifier : STATIC
                              | INLINE
    """
    p[0] = p[1]


def p_declaration_specifier_list_end(p):
    """
        declaration_specifier_list :
    """
    p[0] = []


def p_declaration_specifier_list(p):
    """
        declaration_specifier_list : declaration_specifier_list declaration_specifier
    """
    p[0] = p[1] + [specifiers.Specifier(p.lexer, p.position(2), p[2])]


def p_external_declarations_end(p):
    """
        external_declarations :
    """


def p_external_declarations(p):
    """
        external_declarations : external_declaration external_declarations
    """


def p_external_declarations_namespace(p):
    """
        external_declarations : namespace_declaration external_declarations
    """


def p_external_declaration_empty(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list SEMI
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_type(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list type SEMI
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_variable(p):
    """
        external_declaration : template_specifier_opt variable_declaration SEMI
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_method(p):
    """
        external_declaration : template_specifier_opt method_declaration SEMI
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_method_definition(p):
    """
        external_declaration : template_specifier_opt method_definition
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_using(p):
    """
        external_declaration : USING object_name SEMI
                             | USING NAMESPACE object_name SEMI
    """


def p_typedef_name(p):
    """
        typedef_name : ID
                     | STRUCT_ID_SHADOW
                     | TYPENAME_ID_SHADOW
                     | NAMESPACE_ID_SHADOW
                     | METHOD_ID_SHADOW
                     | VARIABLE_ID_SHADOW
                     | TEMPLATE_STRUCT_ID_SHADOW
                     | TEMPLATE_METHOD_ID_SHADOW
                     | TEMPLATE_TYPENAME_ID_SHADOW
    """


def p_typedef_name_error(p):
    """
        typedef_name : STRUCT_ID
                     | TYPENAME_ID
                     | NAMESPACE_ID
                     | METHOD_ID
                     | VARIABLE_ID
                     | TEMPLATE_STRUCT_ID
                     | TEMPLATE_METHOD_ID
                     | TEMPLATE_TYPENAME_ID
    """


def p_external_declaration_typedef(p):
    """
        external_declaration : TYPEDEF type typedef_name SEMI
    """


def p_external_declaration_error(p):
    """
        external_declaration : error SEMI
    """
