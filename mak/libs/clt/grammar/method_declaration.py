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
    p[0] = [cl_ast.methods.Parameter(p[1], p[2], p[3], [], p.position(2))]


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt COMMA method_parameter_list
    """
    p[0] = [cl_ast.methods.Parameter(p[1], p[2], p[3], [], p.position(2))] + p[5]


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
    if name.qualified:
        if not name.target or name.target.get_token_type() != 'METHOD_ID':
            if len(name.name) > 1:
                p.lexer._error('qualified name %s does not name a method' % '::'.join(name.name), name.position)
            else:
                p.lexer._error('name %s does not name a method' % '::'.join(name.name), name.position)
            if name.target:
                p.lexer._note('previously declared here', name.target.position)
            raise SyntaxError()
        else:
            p[0] = (len(name.name) == 1, name.target)
    else:
        p[0] = (True, cl_ast.methods.Method(p[-1].name[-1], p.position(-1)))
    p.set_position_absolute(0, name.position)


def p_create_constructor(p):
    """
        create_constructor :
    """
    if p.lexer.scopes[-1].definition.constructor:
        p[0] = p.lexer.scopes[-1].definition.constructor
    else:
        p.lexer.scopes[-1].definition.constructor = p[0] = cl_ast.methods.Method(p[-1], p.position(-1))


def p_create_destructor(p):
    """
        create_destructor :
    """
    p[0] = cl_ast.methods.Method('~'+p[-1], p.position(-1))
    if p.lexer.scopes[-1].definition.destructor:
        p.lexer._error('destructor cannot be redeclared', p.position(-1))
    else:
        p.lexer.scopes[-1].definition.destructor = p[0]


def p_create_castop(p):
    """
        create_castop :
    """
    p[0] = cl_ast.methods.Method(p[-1], p.position(-1))


def p_create_method_definition(p):
    """
        create_method_definition :
    """
    p[0] = cl_ast.methods.Body(p.position(-1))
    #p.lexer.scopes[-1].define(p[0])


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


def p_method_attributes(p):
    """
        method_attributes :
                          | method_attribute method_attributes
    """


def p_initializer(p):
    """
        initializer : object_name LPAREN expression_list RPAREN
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
                                  | declaration_specifier_list VOID object_name create_method
    """
    p[0] = (p[2], p[4][0], p[1], p[4][1])
    p.set_position_absolute(0, p[4][1].position)


def p_method_declaration_prefix_cast_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list OPERATOR type create_castop
    """
    p[0] = (p[3], True, p[1], p[4])
    p.set_position(0, 2)


def p_method_declaration_prefix_destructor(p):
    """
        method_declaration_prefix : declaration_specifier_list NOT STRUCT_ID_SHADOW create_destructor
    """
    p[0] = (cl_ast.types.Type(cl_ast.types.Builtin('void', p.position(2)), p.position(2)), True, p[1], p[4])
    p.set_position(0, 2)


def p_method_declaration_prefix_ctor(p):
    """
        method_declaration_prefix : declaration_specifier_list STRUCT_ID_SHADOW create_constructor
                                  | declaration_specifier_list TEMPLATE_STRUCT_ID_SHADOW create_constructor
    """
    p[0] = (cl_ast.types.Type(p.slice[2].found_object, p.position(2)), True, p[1], p[3])
    p.set_position(0, 2)


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix push_method_scope LPAREN method_parameters RPAREN method_attributes pop_method_scope
    """
    is_member = isinstance(p.lexer.scopes[-1], cl_ast.types.Struct) and p.lexer.scopes[-1] or None
    p[0] = p[2].find_overload(p[1][0], p[4], p[1][2], is_member, p[1][1], p.position(1))
    if not p[0]:
        p.lexer._error('Method defined with different signature', p.position(1))


def p_method_definition(p):
    """
        method_definition : method_declaration push_overload_scope create_method_definition initializer_list_opt statement_block pop_method_scope
    """
    p[0] = p[1]

