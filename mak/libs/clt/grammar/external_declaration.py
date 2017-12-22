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

def p_external_declaration_namespace(p):
    """
        external_declaration : namespace_declaration
    """
    p[0] = p[1]

def p_external_declaration_empty(p):
    """
        external_declaration : SEMI
    """
    p[0] = None

def p_external_declaration_type(p):
    """
        external_declaration : type SEMI
    """
    p[0] = p[1]

def p_external_declaration(p):
    """
        external_declaration : variable_declaration SEMI
    """
    p[0] = p[1]

def p_external_declaration_error(p):
    """
        external_declaration : error SEMI
    """
    pass
