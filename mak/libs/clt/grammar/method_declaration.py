from .. import cl_ast


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
    p.set_position(0, 1)


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
    p.lexer._error("redefinition of '%s' as different kind of symbol" % (p[1]), p.position(1))
    p.lexer._note("previous definition is here", p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


def p_method_parameter_type(p):
    """
        method_parameter_type : type
    """
    p[0] = p[1]


def p_method_parameter_list_last(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt
    """
    p[0] = [cl_ast.methods.Parameter(p.lexer.scopes[-1], p.position(2), p[1], p[2], p[3], [])]


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt COMMA method_parameter_list
    """
    p[0] = [cl_ast.methods.Parameter(p.lexer.scopes[-1], p.position(2), p[1], p[2], p[3], [])] + p[5]


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
    name = p[-1]
    template_stack = []
    for container in p.lexer.scopes[::-1]:
        if not isinstance(container, cl_ast.templates.Template):
            break
        template_stack.append(container)
    try:
        cl_ast.templates.resolve(name, template_stack)
    except cl_ast.templates.ResolutionError as e:
        p.lexer._error(str(e), e.position)
        raise SyntaxError()
    if name.qualified:
        if not name.target:
            p.lexer._error('%s should have been declared inside of %s' % ('::'.join(name.name),
                                                                          '::'.join(name.name[:-1])),
                           name.position)
            raise SyntaxError()
        if name.target.get_token_type() not in ('METHOD_ID', 'TEMPLATE_METHOD_ID'):
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name), name.position)
            else:
                p.lexer._error('name %s does not name a method' % '::'.join(name.name), name.position)
            p.lexer._note('previously declared here', name.target.position)
            raise SyntaxError()
        elif name.target.get_token_type() == 'METHOD_ID':
            p[0] = (len(name.name) == 1, name.target)
        else:
            p[0] = (len(name.name) == 1, name.target.specializations[0][1])
    else:
        p[0] = (True, cl_ast.methods.Method(p.lexer.scopes[-1], name.position, p[-1].name[-1]))
        p.lexer.scopes[-1].add(p[0][1])
    p.set_position_absolute(0, name.position)


def p_create_constructor(p):
    """
        create_constructor :
    """
    if p.lexer.scopes[-1].definition.constructor:
        p[0] = p.lexer.scopes[-1].definition.constructor
    else:
        p[0] = cl_ast.methods.SpecialMethod(p.lexer.scopes[-1], p.position(-1), p[-1])
        p.lexer.scopes[-1].definition.constructor = p[0]
    p.set_position(0, -1)


def p_create_castop(p):
    """
        create_castop :
    """
    name = p[-1]
    template_stack = []
    for container in p.lexer.scopes[::-1]:
        if not isinstance(container, cl_ast.templates.Template):
            break
        template_stack.append(container)
    try:
        cl_ast.templates.resolve(name, template_stack)
    except cl_ast.templates.ResolutionError as e:
        p.lexer._error(str(e), e.position)
        raise SyntaxError()

    if name.qualified:
        if not name.target:
            p.lexer._error('%s should have been declared inside of %s' % ('::'.join(name.name),
                                                                          '::'.join(name.name[:-1])),
                           name.position)
            raise SyntaxError()
        if name.target.get_token_type() not in ('METHOD_ID', 'TEMPLATE_METHOD_ID'):
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name), name.position)
            else:
                p.lexer._error('name %s does not name a method' % '::'.join(name.name), name.position)
            p.lexer._note('previously declared here', name.target.position)
            raise SyntaxError()
        elif name.target.get_token_type() == 'METHOD_ID':
            p[0] = (len(name.name) == 1, name.target)
        else:
            p[0] = (len(name.name) == 1, name.target.specializations[0][1])
    else:
        p[0] = (True, cl_ast.methods.Method(p.lexer.scopes[-1], p.position(-1), name.name[-1]))
        p.lexer.scopes[-1].add(p[0][1])
    p.set_position(0, -1)


def p_create_method_definition(p):
    """
        create_method_definition :
    """
    overload = p[-1]
    if overload.definition:
        p.lexer._error('redefinition of method %s' % overload.owner.name, p.position(-1))
        p.lexer._info('originally declared here', overload.position)
        p.lexer._info('first definition here', overload.definition.position)
    overload.definition = cl_ast.methods.Body(p.lexer.scopes[-1], p.position(-1))


def p_push_method_scope(p):
    """
        push_method_scope :
    """
    p[0] = p[-1][3]
    p.lexer.push_scope(p[0])


def p_push_overload_scope(p):
    """
        push_overload_scope :
    """
    p[0] = p[-1]
    p.set_position(0, -1)
    p.lexer.push_scope(p[0])


def p_pop_method_scope(p):
    """
        pop_method_scope :
    """
    p.lexer.pop_scope()


def p_method_attribute(p):
    """
        method_attribute : CONST
    """
    p[0] = cl_ast.expressions.Specifier(p.lexer.scopes[-1], p.position(1), p[1])


def p_method_attributes(p):
    """
        method_attributes :
                          | method_attribute method_attributes
    """
    if len(p) > 1:
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
        method_declaration_prefix : declaration_specifier_list type object_name create_method
    """
    p[0] = (p[2], p[4][0], p[1], p[4][1])
    p.set_position(0, 4)


def p_method_declaration_prefix_void(p):
    """
        method_declaration_prefix : declaration_specifier_list VOID object_name create_method
    """
    p[0] = (None, p[4][0], p[1], p[4][1])
    p.set_position(0, 4)


def p_method_declaration_prefix_cast_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list cast_method_name create_castop
    """
    p[0] = (p[2].data, p[3][0], p[1], p[3][1])
    p.set_position(0, 2)


def p_method_declaration_prefix_ctor(p):
    """
        method_declaration_prefix : declaration_specifier_list special_method_name
    """
    name = p[2]
    template_stack = []
    for container in p.lexer.scopes[::-1]:
        if not isinstance(container, cl_ast.templates.Template):
            break
        template_stack.append(container)
    try:
        cl_ast.templates.resolve(name, template_stack)
    except cl_ast.templates.ResolutionError as e:
        p.lexer._error(str(e), e.position)
        raise SyntaxError()
    if name.qualified:
        if not name.target:
            p.lexer._error('%s should have been declared inside of %s' % ('::'.join(name.name),
                                                                          '::'.join(name.name[:-1])),
                           name.position)
            raise SyntaxError()
        if name.target.get_token_type() not in ('METHOD_ID','SPECIAL_METHOD_ID'):
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name), name.position)
            else:
                p.lexer._error('name %s does not name a method' % '::'.join(name.name), name.position)
            p.lexer._note('previously declared here', name.target.position)
            raise SyntaxError()
        else:
            method = name.target
            p[0] = (method.overloads[0].return_type, len(name.name) == 1, p[1], method)
    elif name.name[-1].startswith('~'):
        method = cl_ast.methods.SpecialMethod(p.lexer.scopes[-1], name.position, name.name[-1])
        if p.lexer.scopes[-1].definition.destructor:
            p.lexer._error('destructor cannot be redeclared', p.position(-1))
        else:
            p.lexer.scopes[-1].definition.destructor = method
        p[0] = (cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(-1), 'void'), True, p[1], method)
    else:
        p.lexer._error('internal error', p.position(-1))
        raise SyntaxError()
    p.set_position_absolute(0, name.position)


def p_method_declaration_prefix_ctor_2(p):
    """
        method_declaration_prefix : declaration_specifier_list STRUCT_ID_SHADOW create_constructor
                                  | declaration_specifier_list TEMPLATE_STRUCT_ID_SHADOW create_constructor
    """
    p[0] = (cl_ast.types.Type(p.lexer.scopes[-1], p.position(2), p.slice[2].found_object), True, p[1], p[3])
    p.set_position(0, 2)


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix push_method_scope LPAREN method_parameters RPAREN method_attributes pop_method_scope
    """
    overload = p[2].find_overload(p.position(1), p[1][0], p[4], p[1][2] + p[6], p[1][1])
    if not overload:
        p.lexer._error('method defined with different signature', p.position(1))
        raise SyntaxError()
    p[0] = overload
    p.set_position(0, 1)


def p_method_definition(p):
    """
        method_definition : method_declaration push_overload_scope create_method_definition initializer_list_opt statement_block pop_method_scope
    """
    p[0] = p[1]

