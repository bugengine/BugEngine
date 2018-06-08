from .. import cl_ast


def p_template_argument(p):
    """
        template_argument : expression                                              %prec PRIO6
                          | type
    """
    p[0] = p[1]


def p_template_argument_list(p):
    """
        template_argument_list : template_argument_list COMMA template_argument
                               | template_argument
    """
    if len(p) > 3:
        p[0] = p[1] + [p[3]]
    else:
        p[0] = [p[1]]


def p_template_arguments(p):
    """
        template_arguments : LT template_argument_list GT                           %prec TEMPLATEGT
    """
    p[0] = p[2]
    p.set_position(0, 3)


def p_template_arguments_empty(p):
    """
        template_arguments : LT GT
    """
    p[0] = []
    p.set_position(0, 2)


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
    p.set_position(0, 1)


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
    p.lexer._error('conflicting declaration %s' % p[1], p.position(1))
    p.lexer._note('previously declared here', p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


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
        template_parameter : template_specifier_opt type template_parameter_name template_parameter_default_value_opt
    """
    p[0] = cl_ast.values.Constant(p[1], p[2], p[3], p.position(2))
    p.lexer.scopes[-1].add_template_parameter(p[0])


def p_template_parameter_typename(p):
    """
        template_parameter : template_specifier_opt TYPENAME template_parameter_name template_parameter_default_value_opt
                           | template_specifier_opt struct_keyword template_parameter_name template_parameter_default_value_opt
    """
    p[0] = cl_ast.types.Typename(p[3], p[4])
    if p[1]:
        p.lexer.pop_scope()
    p.lexer.scopes[-1].add_template_parameter(p[0])


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
    p[0] = cl_ast.templates.Template(p.position(-1))
    p.set_position(0, -1)
    p.lexer.scopes[-1].add(p[0])
    p.lexer.push_scope(p[0])


def p_template_specifier_opt(p):
    """
        template_specifier_opt : TEMPLATE LT template_push template_parameters GT
    """
    p[0] = p[3]
    p[0].position = p.position(2)


def p_template_specifier_none(p):
    """
        template_specifier_opt :
    """
    pass

