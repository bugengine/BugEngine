def p_variable_name_new(p):
    """
        variable_name_new : ID
                          | STRUCT_ID_SHADOW
                          | NAMESPACE_ID_SHADOW
                          | TYPENAME_ID_SHADOW
                          | METHOD_ID_SHADOW
                          | VARIABLE_ID_SHADOW
    """
    p[0] = p[1]
    p.set_position(0, 1)

def p_variable_name_incorrect(p):
    """
        variable_name_new : TYPENAME_ID
                          | NAMESPACE_ID
                          | METHOD_ID
                          | VARIABLE_ID
    """
    p.lexer._error("redefinition of '%s' as different kind of symbol" % (p[1]), p.position(1))
    p.lexer._note("previous definition is here", p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)

def p_variable_initial_value_opt(p):
    """
        variable_initial_value_opt :
    """
    p[0] = None

def p_variable_scope_list_end(p):
    """
        variable_scope_list :
    """
    p[0] = []

def p_variable_scope_list(p):
    """
        variable_scope_list : STATIC variable_scope_list
    """
    p[0] = [p[1]] + p[2]

def p_variable_declaration(p):
    """
        variable_declaration : variable_scope_list type variable_name_new variable_initial_value_opt
    """
    p[0] = p.parser.cl_ast.Variable(p[2], p[3], p[4], p[1], p.position(3))
    p.lexer.scopes[-1].add(p[0])

