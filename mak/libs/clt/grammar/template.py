from .. import cl_ast


def p_consume_template_stack(p):
    """
        consume_template_stack :
    """
    template_stack = []
    for scope in p.lexer.scopes[::-1]:
        if not isinstance(scope.owner, cl_ast.templates.Template):
            break
        template_stack.append(scope.owner)
    p.lexer.push_template_stack(template_stack)


def p_verify_template_stack(p):
    """
        verify_template_stack :
    """
    p[0] = p.lexer.finalize_template_stack()


def p_template_argument(p):
    """
        template_argument : expression                                                  %prec NAME2
                          | type
    """
    p[0] = p[1]


def p_template_argument_list(p):
    """
        template_argument_list : template_argument COMMA template_argument_list         %prec NAME2
                               | template_argument                                      %prec NAME2
    """
    p[0] = [p[1]]
    if len(p) > 2:
        p[0] += p[3]


def p_disable_name_tpl_stack(p):
    """
        disable_name_tpl_stack :
    """
    p.lexer.disable_template_stack()


def p_enable_name_tpl_stack(p):
    """
        enable_name_tpl_stack :
    """
    p.lexer.enable_template_stack()


def p_template_arguments(p):
    """
        template_arguments : LT disable_name_tpl_stack template_argument_list enable_name_tpl_stack GT  %prec TEMPLATEGT
    """
    p[0] = p[3]


def p_template_arguments_empty(p):
    """
        template_arguments : LT GT                                                  %prec TEMPLATEGT
    """
    p[0] = []


def p_template_parameter_name(p):
    """
        template_parameter_name :   ID
                                |   NAMESPACE_ID_SHADOW
                                |   STRUCT_ID_SHADOW
                                |   TYPENAME_ID_SHADOW
                                |   VARIABLE_ID_SHADOW
                                |   METHOD_ID_SHADOW
                                |   TEMPLATE_STRUCT_ID_SHADOW
                                |   TEMPLATE_METHOD_ID_SHADOW
                                |   TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = p[1]


def p_template_parameter_name_incorrect(p):
    """
        template_parameter_name :   NAMESPACE_ID
                                |   STRUCT_ID
                                |   TYPENAME_ID
                                |   VARIABLE_ID
                                |   METHOD_ID
                                |   TEMPLATE_STRUCT_ID
                                |   TEMPLATE_METHOD_ID
                                |   TEMPLATE_TYPENAME_ID
    """
    p.slice(1).found_object
    p[0] = p[1]


def p_template_parameter_default_value_opt(p):
    """
        template_parameter_default_value_opt :  EQUALS expression
                                             |  EQUALS type
                                             |
    """
    if len(p) > 2:
        p[0] = p[2]


def p_template_parameter_value(p):
    """
        template_parameter : template_specifier_opt_detach type template_parameter_name template_parameter_default_value_opt
    """
    if p[1]:
        p.lexer._error("unexpected template specifier", p[1][0].position)
    p[0] = cl_ast.templates.TemplateValueParameter(p.lexer, p.position(3), p[3], p[2], p[4])
    p[0].register()
    for _ in range(0, len(p[1])):
        p.lexer.pop_scope()


def p_template_parameter_typename(p):
    """
        template_parameter : template_specifier_opt_detach TYPENAME template_parameter_name template_parameter_default_value_opt
    """
    if p[1]:
        assert False, "TODO"
    else:
        p[0] = cl_ast.templates.TemplateTypenameParameter(p.lexer, p.position(3), p[3], p[4])
        p[0].register()
    for _ in range(0, len(p[1])):
        p.lexer.pop_scope()


def p_template_parameter_struct(p):
    """
        template_parameter : template_specifier_opt_detach struct_keyword consume_template_stack template_parameter_name verify_template_stack template_parameter_default_value_opt
    """
    if p[1]:
        assert False, "TODO"
    else:
        p[0] = cl_ast.templates.TemplateTypenameParameter(p.lexer, p.position(4), p[4], p[6])
    for _ in range(0, len(p[1])):
        p.lexer.pop_scope()


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


def p_template_push(p):
    """
        template_push :
    """
    p[0] = cl_ast.templates.Template(p.lexer, p.position(-1))
    p[0].register()


def p_template_specifier(p):
    """
        template_specifier : TEMPLATE LT template_push template_parameters GT template_specifier_opt
    """
    p[0] = p[6] + [p[3]]


def p_template_specifier_opt(p):
    """
        template_specifier_opt : template_specifier
    """
    p[0] = p[1]


def p_template_specifier_none(p):
    """
        template_specifier_opt :
    """
    p[0] = []


def p_template_create(p):
    """
        template_create :
    """
    p[0] = cl_ast.templates.Template(p.lexer, p.position(-1))


def p_template_specifier_opt_detach(p):
    """
        template_specifier_opt_detach : TEMPLATE LT template_create template_parameters GT template_specifier_opt_detach
    """
    p[0] = p[6] + [p[3]]


def p_template_specifier_opt_detach_none(p):
    """
        template_specifier_opt_detach :
    """
    p[0]= []
