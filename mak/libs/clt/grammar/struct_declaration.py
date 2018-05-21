from .. import cl_ast


def p_struct_keyword(p):
    """
        struct_keyword : STRUCT
                       | UNION
                       | ENUM
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_declaration(p):
    """
        struct_header : struct_keyword type_name
                      | struct_keyword object_name
    """
    if p[2][1]:
        # qualified name
        if not p[2][2] or p[2][2].get_token_type() != 'STRUCT_ID':
            if len(p[2][0]) > 1:
                p.lexer._error('qualified name %s does not name a struct' % '::'.join(p[2][0]), p.position(2))
            else:
                p.lexer._error('name %s does not name a struct' % '::'.join(p[2][0]), p.position(2))
            if p[2][2]:
                p.lexer._note('previously declared here', p[2][2].position)
            raise SyntaxError()
        else:
            p[0] = (p[2][2], None)
    elif p[2][2] and p[2][2].get_token_type() == 'STRUCT_ID':
        # optimistically use provided declaration, but if definition, it will be overriden
        p[0] = (p[2][2], cl_ast.Struct(p[1], p[2][0][0], p.position(2)))
    else:
        # No previously delcared type, declare one here
        p[0] = (None, cl_ast.Struct(p[1], p[2][0][0], p.position(2)))


def p_struct_push(p):
    """
        struct_push :
    """
    p[0] = p[-3][1] or p[-3][0]
    p[0].define(p[-2])
    p.lexer.scopes[-1].add(p[0])
    p.lexer.push_scope(p[0])


def p_struct_pop(p):
    """
        struct_pop :
    """
    p.lexer.pop_scope()


def p_struct_parent(p):
    """
        struct_parent_opt : COLON type_name
                          | COLON PUBLIC type_name
                          |
    """
    if len(p) > 1:
        p[0] = p[2][2]


def p_struct_parent_error(p):
    """
        struct_parent_opt : COLON object_name
    """
    p.lexer._error('expected class name', p.position(2))


def p_struct_definition(p):
    """
        struct_definition : struct_header struct_parent_opt LBRACE struct_push struct_declaration_list RBRACE struct_pop
        struct_definition : struct_header error LBRACE struct_push struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[4]


def p_struct_header_anonymous(p):
    """
        struct_header_anonymous : struct_keyword LBRACE
    """
    p[0] = cl_ast.Struct(p[1], None, p.position(1))
    p[0].define(None)
    p.lexer.scopes[-1].add(p[0])
    p.lexer.push_scope(p[0])


def p_struct_definition_anonymous(p):
    """
        struct_definition : struct_header_anonymous struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]


def p_type_struct_declaration(p):
    """
        typedecl : struct_header
    """
    p[0] = p[1][0] or p[1][1]
    p.lexer.scopes[-1].add(p[0])


def p_type_struct_definition(p):
    """
        typedecl : struct_definition
    """
    p[0] = p[1]
    p.lexer.scopes[-1].add(p[0])


def p_struct_declaration_list(p):
    """
        struct_declaration_list : external_declaration struct_declaration_list
                                | struct_declaration struct_declaration_list
    """
    pass


def p_struct_declaration_list_end(p):
    """
        struct_declaration_list :
    """
    pass


def p_struct_declaration_scope(p):
    """
        struct_declaration : PUBLISHED COLON
                           | PUBLIC COLON
                           | PROTECTED COLON
                           | PRIVATE COLON
                           | FRIEND type SEMI
    """
    pass

