from .. import cl_ast


def p_declaration_specifier(p):
    """
        declaration_specifier : STATIC
                              | INLINE
    """
    p[0] = cl_ast.expressions.Specifier(p[1], p.position(1))


def p_declaration_specifier_list_end(p):
    """
        declaration_specifier_list :
    """
    p[0] = []


def p_declaration_specifier_list(p):
    """
        declaration_specifier_list : declaration_specifier_list declaration_specifier
    """
    p[0] = p[1] + [p[2]]
    p.set_position(0, 2)


def p_external_declarations_end(p):
    """
        external_declarations :
    """
    p[0] = []


def p_external_declarations(p):
    """
        external_declarations : external_declarations external_declaration
    """
    p[0] = p[1]
    if p[2]:
        p[0].append(p[2])


def p_external_declarations_namespace(p):
    """
        external_declarations : external_declarations namespace_declaration
    """
    p[0] = p[1]
    if p[2]:
        p[0].append(p[2])


def p_external_declaration_empty(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list SEMI
    """
    p[0] = None
    if p[1]:
        p.lexer.pop_scope()


def p_external_declaration_type(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list type SEMI
    """
    p[0] = p[3]
    for s in p[2]:
        p.lexer._warning('specifier ignored', s.position)
    if p[1]:
        p.lexer.pop_scope()


def p_external_declaration_variable(p):
    """
        external_declaration : template_specifier_opt variable_declaration SEMI
    """
    p[0] = p[2]
    if p[1]:
        for variable in p[2]:
            p.lexer._error('Variable %s declared as a template' % variable.name, variable.position)
        p.lexer.pop_scope()


def p_external_declaration_method(p):
    """
        external_declaration : template_specifier_opt method_declaration SEMI
    """
    if p[1]:
        p.lexer.pop_scope()


def p_external_declaration_method_definition(p):
    """
        external_declaration : template_specifier_opt method_definition
    """
    if p[1]:
        p.lexer.pop_scope()


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
    p[0] = p[1]
    p.set_position(0, 1)


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
    p.lexer._error("redefinition of '%s' as different kind of symbol" % (p[1]), p.position(1))
    p.lexer._note("previous definition is here", p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


def p_external_declaration_typedef(p):
    """
        external_declaration : TYPEDEF type typedef_name SEMI
    """
    p.lexer.scopes[-1].add(cl_ast.types.Typedef(p[3], p[2], p.position(3)))


def p_external_declaration_error(p):
    """
        external_declaration : error SEMI
                             | error statement_block
    """
    pass
