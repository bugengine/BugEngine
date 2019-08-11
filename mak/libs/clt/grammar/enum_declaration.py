from ..cl_ast.enum import Enum, EnumItem


def p_enum_declaration(p):
    """
        enum_header : ENUM consume_template_stack type_name verify_template_stack
                    | ENUM consume_template_stack template_name verify_template_stack
                    | ENUM consume_template_stack object_name verify_template_stack
    """
    name = p[3]
    if name.qualified:
        if name.target:
            if name.target.get_token_type() == 'STRUCT_ID':
                if name.target.struct_type != p[1]:
                    p.lexer._error("'%s' declared as %s here, but first declared as %s" % (name, p[1], name.target.struct_type), p.position(1))
                    p.lexer._note('previously declared here', name.target.position)
                    p[0] = (None, Enum(p.lexer, name.position, name.name[-1]))
                else:
                    p[0] = (name.target, None)
            else:
                if len(name.name) > 1:
                    p.lexer._error('qualified name %s does not name an enum' % '::'.join(name.name),
                                   name.position)
                else:
                    p.lexer._error('name %s does not name an enum' % '::'.join(name.name),
                                   name.position)
                p.lexer._note('previously declared here', name.target.position)
                p[0] = (None, Enum(p.lexer, name.position, name.name[-1]))
        elif name.targets[-1][2]:
            p.lexer._error('enumeration cannot be a template', p.position(1))
            p[0] = (None, Enum(p.lexer, name.position, name.name[-1]))
        else:
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name an enum' % '::'.join(name.name),
                               name.position)
            else:
                p.lexer._error('name %s does not name an enum' % '::'.join(name.name),
                               name.position)
            p[0] = (None, Enum(p.lexer, name.position, name.name[-1]))
    elif name.target:
        # optimistically use provided declaration, but if definition, it will be overriden
        if p[4]:
            p.lexer._error('enumeration cannot be a template', p.position(1))
            p[0] = (None, Enum(p.lexer, name.position, name.name[0]))
        if name.target.get_token_type() == 'STRUCT_ID':
            if name.target.struct_type == p[1]:
                p[0] = (name.target, Enum(p.lexer, name.position, name.name[0]))
            else:
                p[0] = (None, Enum(p.lexer, name.position, name.name[0]))
        else:
            # Previously declared object is not an enum
            p[0] = (None, Enum(p.lexer, name.position, name.name[0]))
    else:
        if name.targets[-1][2]:
            # new specialization
            p.lexer._error('enumeration cannot be a template', p.position(1))
            p[0] = (None, Enum(p.lexer, name.position, name.name[0]))
        else:
            # No previously delcared type, declare one here
            if p[4]:
                p.lexer._error('enumeration cannot be a template', p.position(1))
            p[0] = (None, Enum(p.lexer, name.position, name.name[0]))
    p.set_position_absolute(0, name.position)


def p_enum_push(p):
    """
        enum_push :
    """
    p[0] = p[-1][1]
    if not p[0]:
        p[0] = p[-1][0]
    else:
        p[0].register()
    p[0].define(p.position(-2))


def p_enum_pop(p):
    """
        enum_pop :
    """
    enum = p[-3]
    p.lexer.pop_scope(enum.scope)


def p_enum_begin(p):
    """
        enum_begin : enum_header enum_push LBRACE
    """
    p[0] = p[2]
    p.set_position(0, 1)


def p_enum_definition(p):
    """
        enum_definition : enum_begin enum_declaration_list RBRACE enum_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_header_anonymous(p):
    """
        enum_header_anonymous : ENUM consume_template_stack verify_template_stack LBRACE
    """
    p[0] = Enum(p.lexer, p.position(1), None)
    p[0].register()
    p[0].define(p.position(4))
    p.set_position(0, 1)


def p_enum_definition_anonymous(p):
    """
        enum_definition : enum_header_anonymous enum_declaration_list RBRACE enum_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_declaration_list(p):
    """
        enum_declaration_list : enum_declaration_list_nonempty
                              |
    """


def p_enum_declaration_list_nonempty(p):
    """
        enum_declaration_list_nonempty : enum_item COMMA enum_declaration_list_nonempty
                                       | enum_item
    """


def p_enum_value_name(p):
    """
        enum_value_name : ID
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_name_shadows(p):
    """
        enum_value_name : METHOD_ID_SHADOW
                        | VARIABLE_ID_SHADOW
                        | STRUCT_ID_SHADOW
                        | TYPENAME_ID_SHADOW
    """
    if p.slice[1].found_object:
        owner = p.slice[1].found_object.parent
        enum_namespace = p.lexer.scopes[-2].owner
        if owner == enum_namespace:
            p.lexer._error("redefinition of '%s'" % p[1], p.position(1))
            p.lexer._note('previous definition is here', p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_error(p):
    """
        enum_value_name : VARIABLE_ID
    """
    # TODO
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_value_optional(p):
    """
        enum_value_value_optional : EQUALS expression
                                  |
    """
    if len(p) > 1:
        p[0] = p[2]


def p_enum_declaration_item(p):
    """
        enum_item : enum_value_name enum_value_value_optional
    """
    p[0] = EnumItem(p.lexer, p.position(1), p[1], p[2])
    p[0].register()


def p_type_enum_declaration(p):
    """
        typedecl : enum_header
    """
    p[0] = p[1][0]
    if not p[0]:
        p[0] = p[1][1]
        p[0].register()
    p.set_position(0, 1)


def p_type_enum_definition(p):
    """
        typedecl : enum_definition
    """
    p[0] = p[1]
    p.set_position(0, 1)
