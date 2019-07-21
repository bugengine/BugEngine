from .. import cl_ast


def p_struct_keyword(p):
    """
        struct_keyword : STRUCT
                       | UNION
                       | ENUM
                       | CLASS
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_declaration(p):
    """
        struct_header : struct_keyword consume_template_stack type_name verify_template_stack
                      | struct_keyword consume_template_stack object_name verify_template_stack
    """
    name = p[3]
    if name.qualified:
        if name.target:
            if name.target.get_token_type() == 'STRUCT_ID':
                p[0] = (name.target, None)
            elif name.target.get_token_type() == 'TEMPLATE_STRUCT_ID':
                assert False, 'Name parsing should have handled this'
                # retrieve right specialization
                template = p[4]
                if template:
                    template.bind(name.target)
                    cls = name.target.find_specialization(name.position, template.parameters)
                else:
                    p.lexer._error('template specialization or definition requires a '
                                   'template parameter list corresponding to '
                                   'the nested type %s' % '::'.join(name.name), name.position)
                    cls = name.target.scope.items[0][1]
                p[0] = (cls, None)
            else:
                if len(name.name) > 1:
                    p.lexer._error('qualified name %s does not name a struct' % '::'.join(name.name),
                                   name.position)
                else:
                    p.lexer._error('name %s does not name a struct' % '::'.join(name.name),
                                   name.position)
                p.lexer._note('previously declared here', name.target.position)
                p[0] = (None, cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0]))
        elif name.targets[-1][2]:
            # new specialization
            s = cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0])
            name.targets[-1][2].create_specialization(name.targets[-1][1], s)
            p[0] = (s, None)
        else:
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a struct' % '::'.join(name.name),
                               name.position)
            else:
                p.lexer._error('name %s does not name a struct' % '::'.join(name.name),
                               name.position)
            p[0] = (None, cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0]))
    elif name.target:
        # optimistically use provided declaration, but if definition, it will be overriden
        if p[4]:
            p[4].bind(p[4])
        if name.target.get_token_type() == 'STRUCT_ID':
            p[0] = (name.target, cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0]))
        elif name.target and name.target.get_token_type() == 'TEMPLATE_STRUCT_ID':
            p[0] = (name.target.specializations[0][1], cl_ast.types.Struct(p.lexer, name.position,
                                                                           p[1], name.name[0]))
        else:
            # Previously declared object is not a type
            p[0] = (None, cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0]))
    else:
        if name.targets[-1][2]:
            # new specialization
            s = cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0])
            name.targets[-1][2].create_specialization(name.targets[-1][1], s)
            p[0] = (s, None)
        else:
            # No previously delcared type, declare one here
            if p[4]:
                p[4].bind(p[4])
            p[0] = (None, cl_ast.types.Struct(p.lexer, name.position, p[1], name.name[0]))
    p.set_position_absolute(0, name.position)


def p_struct_push(p):
    """
        struct_push :
    """
    p[0] = p[-2][1]
    if not p[0]:
        p[0] = p[-2][0]
    else:
        p[0].register()
    p[0].define(p[-1], p.position(-2))


def p_struct_pop(p):
    """
        struct_pop :
    """
    struct = p[-3]
    p.lexer.pop_scope(struct.scope)


def p_struct_parent_visibility(p):
    """
        struct_parent_visibility_opt : PUBLIC
                                     | PROTECTED
                                     | PRIVATE
                                     |
    """
    if len(p) > 1:
        p[0] = p[1]


def p_struct_parent(p):
    """
        struct_parent_opt : COLON struct_parent_visibility_opt type_name
                          |
    """
    if len(p) > 1:
        p[0] = p[3].target


def p_struct_parent_error(p):
    """
        struct_parent_opt : COLON struct_parent_visibility_opt object_name
    """
    p.lexer._error('expected class name', p[3].position)


def p_struct_begin(p):
    """
        struct_begin : struct_header struct_parent_opt struct_push LBRACE
    """
    p[0] = p[3]
    p.set_position(0, 1)


def p_struct_definition(p):
    """
        struct_definition : struct_begin struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_header_anonymous(p):
    """
        struct_header_anonymous : struct_keyword consume_template_stack verify_template_stack LBRACE
    """
    p[0] = cl_ast.types.Struct(p.lexer, p.position(1), p[1], None)
    p[0].register()
    p[0].define(None, p.position(4))
    p.set_position(0, 1)


def p_struct_definition_anonymous(p):
    """
        struct_definition : struct_header_anonymous struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_type_struct_declaration(p):
    """
        typedecl : struct_header
    """
    p[0] = p[1][0]
    if not p[0]:
        p[0] = p[1][1]
        p[0].register()
    p.set_position(0, 1)


def p_type_struct_definition(p):
    """
        typedecl : struct_definition
    """
    p[0] = p[1]
    p.set_position(0, 1)


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
    """
    pass

def p_struct_declaration_friend(p):
    """
        struct_declaration : FRIEND struct_keyword type_name SEMI
    """
    pass

def p_struct_declaration_friend_template(p):
    """
        struct_declaration : template_specifier FRIEND struct_header SEMI
    """
    for t in p[1]:
        p.lexer.pop_scope(t.scope)
