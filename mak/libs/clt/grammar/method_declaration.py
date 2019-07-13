from ..cl_ast import methods, types


def p_parameter_name_none(p):
    """
        parameter_name :
    """
    p[0] = None


def p_parameter_name(p):
    """
        parameter_name : ID
                       | STRUCT_ID_SHADOW
                       | TYPENAME_ID_SHADOW
                       | NAMESPACE_ID_SHADOW
                       | METHOD_ID_SHADOW
                       | VARIABLE_ID_SHADOW
                       | TEMPLATE_STRUCT_ID_SHADOW
                       | TEMPLATE_METHOD_ID_SHADOW
                       | TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = p[1]


def p_parameter_name_invalid(p):
    """
        parameter_name : STRUCT_ID
                       | TYPENAME_ID
                       | NAMESPACE_ID
                       | METHOD_ID
                       | VARIABLE_ID
                       | TEMPLATE_STRUCT_ID
                       | TEMPLATE_METHOD_ID
                       | TEMPLATE_TYPENAME_ID
    """
    p._lexer.error('redefinition of %s' % p[0])
    p.slice[1].found_object._note('previously declared here')
    p[0] = p[1]


def p_method_parameter_type(p):
    """
        method_parameter_type : type
    """
    p[0] = p[1]


def p_method_parameter_list_last(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt
    """
    p[0] = [methods.Parameter(p.lexer, p[1].position, p[2], p[1], p[3])]


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt COMMA method_parameter_list
    """
    p[0] = [methods.Parameter(p.lexer, p[1].position, p[2], p[1], p[3])] + p[5]


def p_method_parameters(p):
    """
        method_parameters : method_parameter_list
    """
    p[0] = p[1]


def p_method_parameters_none(p):
    """
        method_parameters : VOID
                          |
    """
    p[0] = []


def p_create_method(p):
    """
        create_method :
    """
    name = p[-2]
    template = p[-1]
    if name.qualified:
        if name.target:
            if name.target.get_token_type() == 'METHOD_ID':
                p[0] = name.target
            elif name.target.get_token_type() == 'TEMPLATE_METHOD_ID':
                assert False, 'Name parsing should have handled this'
                # retrieve right specialization
                if template:
                    template.bind(name.target)
                    method = name.target.find_specialization(name.position, template.parameters)
                else:
                    p.lexer._error('template specialization or definition requires a '
                                   'template parameter list corresponding to '
                                   'the nested type %s' % '::'.join(name.name), name.position)
                    method = name.target.scope.items[0][1]
                p[0] = method
            else:
                if len(name.name) > 1:
                    p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name),
                                   name.position)
                else:
                    p.lexer._error('name %s does not name a method' % '::'.join(name.name),
                                   name.position)
                p.lexer._note('previously declared here', name.target.position)
                p[0] = methods.Method(p.lexer, name.position, name.name[-1])
                p[0].register()
        elif name.targets[-1][2]:
            # new specialization
            m = methods.Method(p.lexer, name.position, name.name[-1])
            name.targets[-1][2].create_specialization(name.targets[-1][1], m)
            p[0] = m
            p[0].register()
        else:
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name),
                               name.position)
            else:
                p.lexer._error('name %s does not name a method' % '::'.join(name.name),
                               name.position)
            p[0] = methods.Method(p.lexer, name.position, name.name[-1])
            p[0].register()
    elif name.target:
        # method not in this scope: redeclare
        print(name.name, name.target, name.target.position)
        if template:
            template.bind(template)
        p[0] = methods.Method(p.lexer, name.position, name.name[-1])
        p[0].register()
    else:
        if name.targets[-1][2]:
            assert False, "TODO"
        else:
            # No previously delcared type, declare one here
            if template:
                template.bind(template)
            p[0] = methods.Method(p.lexer, name.position, name.name[-1])
            p[0].register()
    p.set_position_absolute(0, name.position)


def p_create_special_method(p):
    """
        create_special_method :
    """
    name = p[-1]
    klass = name.data
    obj = name.target
    if obj:
        p[0] = obj
    else:
        assert name.name[-1][0] == '~'
        assert klass, 'resolution of %s failed' % name
        assert klass.scope, '%s is not defined' % name
        if klass.scope.destructor:
            p[0] = klass.scope.destructor
        else:
            p[0] = klass.scope.destructor = methods.SpecialMethod(p.lexer, name.position, name.name[-1], klass)


def p_create_castop(p):
    """
        create_castop :
    """
    name = p[-1]
    cast_type = name.data
    if len(name.targets) > 1:
        owner = name.targets[-2]
    else:
        owner = p.lexer.scopes[-1].owner
    assert isinstance(owner, types.Struct)
    for type, method in owner.scope.casts:
        try:
            type.distance(cast_type, types.CAST_NONE)
        except types.CastError:
            continue
        else:
            p[0] = method
            break
    else:
        p[0] = methods.Method(p.lexer, name.position, name.name[-1])
        owner.scope.casts.append((name.data, p[0]))


def p_push_overload_scope(p):
    """
        push_overload_scope :
    """


def p_pop_overload_scope(p):
    """
        pop_overload_scope :
    """


def p_method_attribute(p):
    """
        method_attribute : CONST
    """
    p[0] = p[1]


def p_method_attributes(p):
    """
        method_attributes :
                          | method_attribute method_attributes
    """
    if len(p) > 2:
        p[0] = [p[1]] + p[2]
    else:
        p[0] = []


def p_initializer(p):
    """
        initializer : object_name LPAREN expression_list_opt RPAREN
    """


def p_initializer_list(p):
    """
        initializer_list : initializer
                         | initializer COMMA initializer_list
    """


def p_initializer_list_opt(p):
    """
        initializer_list_opt : COLON initializer_list
                             |
    """


def p_method_declaration_prefix(p):
    """
        method_declaration_prefix : declaration_specifier_list type consume_template_stack object_name verify_template_stack create_method
    """
    p[0] = (p[2], p[1], p[6], p[4])


def p_method_declaration_prefix_void(p):
    """
        method_declaration_prefix : declaration_specifier_list VOID consume_template_stack object_name verify_template_stack create_method
    """
    p[0] = (None, p[1], p[6], p[4])


def p_method_declaration_prefix_cast_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list cast_method_name create_castop
    """
    p[0] = (p[2].data, p[1], p[3], p[2])


def p_method_declaration_prefix_special_method(p):
    """
        method_declaration_prefix : declaration_specifier_list special_method_name create_special_method
    """
    p[0] = (None, p[1], p[3], p[2])


def p_method_declaration_prefix_ctor_2(p):
    """
        method_declaration_prefix : declaration_specifier_list STRUCT_ID_SHADOW
                                  | declaration_specifier_list TEMPLATE_STRUCT_ID_SHADOW
    """
    name = p[2]
    klass = p.slice[2].found_object
    assert klass, 'resolution of %s failed' % name
    assert klass.scope
    if klass.scope.constructor:
        m = klass.scope.constructor
    else:
        m = klass.scope.constructor = methods.SpecialMethod(p.lexer, p.position(2), name, klass)
    p[0] = (None, p[1], m, p[2])


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix LPAREN method_parameters RPAREN method_attributes
    """
    if p[1][0]:
        assert isinstance(p[1][0], types.TypeRef), p[1][3].name
    p[0] = p[1][2].find_overload(p[3], p[1][0], p[1][1] + p[5])
    if not p[0]:
        p[0] = p[1][2].create_overload(p.position(2), p[3], p[1][0], p[1][1] + p[5])


def p_method_definition(p):
    """
        method_definition : method_declaration push_overload_scope initializer_list_opt statement_block pop_overload_scope
    """

