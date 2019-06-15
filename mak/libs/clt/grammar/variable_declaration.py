from ..cl_ast import types, variables


def p_variable_initial_value_opt(p):
    """
        variable_initial_value_opt :
    """
    p[0] = None


def p_variable_initial_value(p):
    """
        variable_initial_value_opt : EQUALS expression
    """
    p[0] = None


def p_type_modifier_opt_end(p):
    """
        type_modifier_opt :
    """
    p[0] = p[-1]


def p_type_modifier_opt(p):
    """
        type_modifier_opt : type_modifier_opt TIMES
    """
    p[0] = types.Pointer(p.lexer, p.position(2), p[1])


def p_variable_array_specifier_opt_end(p):
    """
        variable_array_specifier_opt :
    """
    p[0] = p[-4]


def p_variable_array_specifier_opt(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET expression RBRACKET
    """
    p[0] = types.Array(p.lexer, p.position(2), p[-4], p[3])


def p_variable_array_specifier_opt_error(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET RBRACKET
    """
    p[0] = types.Pointer(p.lexer, p.position(2), p[1])


def p_variable_declaration(p):
    """
        variable_declaration : declaration_specifier_list type consume_template_stack object_name verify_template_stack variable_array_specifier_opt variable_initial_value_opt
    """
    name = p[4]
    if name.qualified:
        p[0] = [name.target]
        # todo: check specifiers
    else:
        v = variables.Variable(p.lexer, name.position, name.name[0], p[6], p[7], p[1])
        p[0] = [v]
        p.lexer.scopes[-1].add(p[0][0])
        for s in p[1]:
            if s.specifier == 'inline':
                p.lexer._error('inline can only be used on functions', s.position)


def p_extract_original_type(p):
    """
        extract_original_type :
    """
    p[0] = p[-2][0].type.original_type()


def p_variable_declaration_cted(p):
    """
        variable_declaration : variable_declaration COMMA extract_original_type type_modifier_opt consume_template_stack object_name verify_template_stack variable_array_specifier_opt variable_initial_value_opt
    """
    name = p[6]
    if name.qualified:
        p[0] = p[1] + [name.target]
        # todo: check specifiers
    else:
        v = variables.Variable(p.lexer, name.position, name.name[0], p[8], p[9], p[1][0].attributes)
        p[0] = p[1] + [v]
