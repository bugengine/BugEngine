from .. import cl_ast

def p_template_parameter_name_opt(p):
    """
        template_parameter_name_opt :   ID
                                    |   NAMESPACE_ID_SHADOW
                                    |   STRUCT_ID_SHADOW
                                    |   TYPENAME_ID_SHADOW
                                    |   VARIABLE_ID_SHADOW
                                    |   METHOD_ID_SHADOW
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
