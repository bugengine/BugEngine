def p_struct_keyword(p):
    """
        struct_keyword : STRUCT
                       | UNION
                       | ENUM
    """
    p[0] = p[1]

def p_struct_name_new(p):
    """
        struct_name_new : ID
                        | TYPENAME_ID
                        | STRUCT_ID_SHADOW
                        | NAMESPACE_ID_SHADOW
                        | TYPENAME_ID_SHADOW
                        | METHOD_ID_SHADOW
                        | VARIABLE_ID_SHADOW
    """
    p[0] = p[1]
    p.set_position(0, 1)

def p_struct_name_incorrect(p):
    """
        struct_name_new : NAMESPACE_ID
                        | METHOD_ID
                        | VARIABLE_ID
    """
    p.lexer._error("redefinition of '%s' as different kind of symbol" % (p[1]), p.position(1))
    p.lexer._note("previous definition is here", p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)

def p_struct_name_existing(p):
    """
        struct_name_existing : STRUCT_ID
    """
    p[0] = p[1]

def p_struct_begin_new(p):
    """
        struct_begin_new : struct_keyword struct_name_new
    """
    p[0] = p.parser.cl_ast.Struct(p[1], p[2], p.position(2))

def p_struct_begin_existing(p):
    """
        struct_begin_existing : struct_keyword struct_name_existing
    """
    p[0] = p[2].found_object

def p_struct_declaration(p):
    """
        struct_header : struct_begin_new
                      | struct_begin_existing
    """
    p[0] = p[1]
    p.lexer.scopes[-1].add(p[0])

def p_struct_push(p):
    """
        struct_push :
    """
    p[-3].define(p[-2])
    p.lexer.scopes.append(p[-3])

def p_struct_pop(p):
    """
        struct_pop :
    """
    p.lexer.scopes.pop(-1)

def p_struct_parent_opt(p):
    """
        struct_parent_opt :
    """
    p[0] = None

def p_struct_parent(p):
    """
        struct_parent_opt : COLON STRUCT_ID
    """
    p[0] = p[2].found_object

def p_struct_specifier(p):
    """
        struct_specifier : struct_header struct_parent_opt LBRACE struct_push struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]


def p_type_struct_declaration(p):
    """
        type : struct_specifier
             | struct_header
    """
    p[0] = p[1]


def p_struct_declaration_list(p):
    """
        struct_declaration_list : struct_declaration struct_declaration_list
    """
    pass

def p_struct_declaration_list_end(p):
    """
        struct_declaration_list :
    """
    pass

def p_struct_declaration_empty(p):
    """
        struct_declaration : SEMI
    """
    p[0] = None

def p_struct_declaration_type(p):
    """
        struct_declaration : type SEMI
    """
    p[0] = p[1]

def p_struct_declaration_variable(p):
    """
        struct_declaration : variable_declaration SEMI
    """
    p[0] = p[1]

def p_struct_declaration_error(p):
    """
        struct_declaration : error SEMI
    """
    pass
