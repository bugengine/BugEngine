from .. import cl_ast


def p_template_parameter_name_opt(p):
    """
        template_parameter_name_opt :   ID
                                    |   NAMESPACE_ID_SHADOW
                                    |   STRUCT_ID_SHADOW
                                    |   TYPENAME_ID_SHADOW
                                    |   VARIABLE_ID_SHADOW
                                    |   METHOD_ID_SHADOW
                                    |   TEMPLATE_ID_SHADOW
                                    |
    """
    if len(p) > 1:
        p[0] = p[1]


def p_template_parameter_default_value_opt(p):
    """
        template_parameter_default_value_opt :  EQUALS expression   %prec PRIO11
                                             |  EQUALS type
                                             |
    """
    if len(p) > 1:
        p[0] = p[1]


def p_template_parameter_value(p):
    """
        template_parameter : type template_parameter_name_opt template_parameter_default_value_opt
    """
    p[0] = cl_ast.Variable(p[1], p[2], p[3])


def p_template_parameter_typename(p):
    """
        template_parameter : TYPENAME template_parameter_name_opt template_parameter_default_value_opt
    """
    p[0] = cl_ast.Typename(p[2], p[3])


def p_template_parameter_list(p):
    """
        template_parameter_list : template_parameter_list COMMA template_parameter
                                | template_parameter
    """
    if len(p) > 2:
        p[0] = p[1] + [p[3]]
    else:
        p[0] = [p[1]]


def p_template_parameters(p):
    """
        template_parameters : template_parameter_list
    """
    p[0] = p[1]


def p_template_parameters_none(p):
    """
        template_parameters :
    """
    p[0] = []


def p_template_specifier(p):
    """
        template_specifier_opt : TEMPLATE LT template_parameters GT
    """
    p[0] = cl_ast.Template(p[3])
    p.set_position(0, 2)
    p.lexer.scopes[-1].add(p[0])
    p.lexer.scopes.append(p[0])


def p_template_specifier_opt(p):
    """
        template_specifier_opt :
    """
    pass


def p_declaration_specifier(p):
    """
        declaration_specifier : STATIC
                              | INLINE
    """
    p[0] = cl_ast.Specifier(p[1], p.position(1))


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
    p[0] = p[1] + (p[2] and [p[2]] or [])


def p_external_declarations_namespace(p):
    """
        external_declarations : external_declarations namespace_declaration
    """
    p[0] = p[1] + (p[2] and [p[2]] or [])


def p_external_declaration_empty(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list SEMI
    """
    p[0] = None
    if p[1]:
        p.lexer.scopes.pop(-1)


def p_external_declaration_type(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list type SEMI
    """
    p[0] = p[3]
    for s in p[2]:
        p.lexer._warning('specifier ignored', s.position)
    if p[1]:
        p.lexer.scopes.pop(-1)


def p_external_declaration_variable(p):
    """
        external_declaration : template_specifier_opt variable_declaration SEMI
    """
    p[0] = p[2]
    if p[1]:
        for variable in p[2]:
            p.lexer._error('Variable %s declared as a template' % variable.name, variable.position)
        p.lexer.scopes.pop(-1)


def p_external_declaration_method(p):
    """
        external_declaration : template_specifier_opt method_declaration SEMI
    """
    if p[1]:
        p.lexer.scopes.pop(-1)


def p_external_declaration_method_definition(p):
    """
        external_declaration : template_specifier_opt method_definition
    """
    if p[1]:
        p.lexer.scopes.pop(-1)


def p_external_declaration_error(p):
    """
        external_declaration : error SEMI
    """
    pass
