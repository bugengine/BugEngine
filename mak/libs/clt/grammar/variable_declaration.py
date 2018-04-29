from .. import cl_ast


def p_variable_initial_value_opt(p):
    """
        variable_initial_value_opt :
    """
    p[0] = None


def p_variable_initial_value(p):
    """
        variable_initial_value_opt : EQUALS expression %prec PRIO11
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
    p[0] = cl_ast.Pointer(p[1], p.position(2))


def p_variable_array_specifier_opt_end(p):
    """
        variable_array_specifier_opt :
    """
    p[0] = p[-2]


def p_variable_array_specifier_opt(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET expression RBRACKET
    """
    p[0] = cl_ast.Array(p[-2], p[3], p.position(2))


def p_variable_array_specifier_opt_error(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET RBRACKET
    """
    p[0] = cl_ast.Pointer(p[1], p.position(2))


def p_variable_declaration(p):
    """
        variable_declaration : declaration_specifier_list type object_name variable_array_specifier_opt variable_initial_value_opt
                             | declaration_specifier_list typedecl object_name variable_array_specifier_opt variable_initial_value_opt
    """
    if p[3][1]:
        p[0] = [p[3][1]]
        # todo: check specifiers
    else:
        p[0] = [cl_ast.Variable(p[4], p[3][0], p[5], p[1], p.position(3))]
        p.lexer.scopes[-1].add(p[0][0])
        for s in p[1]:
            if s.specifier == 'inline':
                p.lexer._error('inline can only appear on functions', s.position)


def p_extract_original_type(p):
    """
        extract_original_type :
    """
    p[0] = p[-2][0].type.original_type()


def p_variable_declaration_cted(p):
    """
        variable_declaration : variable_declaration COMMA extract_original_type type_modifier_opt object_name variable_array_specifier_opt variable_initial_value_opt
    """
    if p[5][1]:
        p[0] = p[1] + [p[5][1]]
        # todo: check specifiers
    else:
        p[0] = p[1] + [cl_ast.Variable(p[6], p[5][0], p[7], p[1][0].attributes, p.position(5))]
        p.lexer.scopes[-1].add(p[0][-1])
