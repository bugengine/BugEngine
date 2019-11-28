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
    p[0] = p[2]


def p_variable_initial_value_constructor(p):
    """
        variable_initial_value_opt : LPAREN expression_list RPAREN
    """
    p[0] = p[2]


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
    p[0] = p[-3]


def p_variable_array_specifier_opt(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET expression RBRACKET
    """
    p[0] = types.Array(p.lexer, p.position(2), p[1], p[3])


def p_variable_array_specifier_opt_error(p):
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET RBRACKET
    """
    p[0] = types.Pointer(p.lexer, p.position(2), p[1])


def p_variable_declaration(p):
    """
        variable_declaration : object_declaration variable_initial_value_opt
    """
    attributes, _, name, owner, variable_type = p[1]
    owner.pop_scope_recursive()
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if object_type != 'VARIABLE_ID':
            p.lexer.error('%s redeclared as different kind of symbol', name.position)
            p.lexer.note('previous declaration of %s' % name.target.position)
            p[0] = variables.Variable(p.lexer, name.position, name.name, variable_type, p[2], attributes)
        else:
            try:
                name.target.type.distance(variable_type, types.CastOptions(types.CastOptions.CAST_NONE))
            except types.CastError:
                p.lexer.error("redefinition of '%s' with a different type: '%s' vs '%s'" % (name, variable_type.pretty_name(), name.target.type.pretty_name()))
                p.lexer.note('previous declaration of %s' % name.target.position)
            p[0] = [name.target]
    else:
        v = variables.Variable(p.lexer, name.position, name.name, variable_type, p[2], attributes)
        v.register()
        p[0] = [v]
        for s in attributes:
            if s.specifier == 'inline':
                p.lexer.error('inline can only be used on functions', s.position)


def p_extract_original_type(p):
    """
        extract_original_type :
    """
    p[0] = p[-2][0].type.original_type()


def p_variable_declaration_cted(p):
    """
        variable_declaration : variable_declaration COMMA extract_original_type type_modifier_opt object_name variable_array_specifier_opt variable_initial_value_opt
    """
    name = p[5][1]
    object_type = name.get_type()
    variable_type = p[6]
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if name.get_type() != 'VARIABLE_ID':
            p.lexer.error('%s redeclared as different kind of symbol', name.position)
            p.lexer.note('previous declaration of %s' % name.target.position)
            p[0] = variables.Variable(p.lexer, name.position, name.name, variable_type, p[7], p[1][0].attributes)
        else:
            try:
                name.target.distance(variable_type, types.CastOptions(types.CastOptions.CAST_NONE))
            except types.CastError:
                p.lexer.error("redefinition of '%s' with a different type: '%s' vs '%s'" % (name, variable_type.pretty_name(), name.target.type.pretty_name()))
                p.lexer.note('previous declaration of %s' % name.target.position)
            p[0] = [name.target]
    else:
        v = variables.Variable(p.lexer, name.position, name.name, variable_type, p[7], p[1][0].attributes)
        v.register()
        p[0] = [v]
        for s in p[1]:
            if s.specifier == 'inline':
                p.lexer.error('inline can only be used on functions', s.position)
