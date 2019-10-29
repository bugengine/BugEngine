from ..cl_ast import methods, types, templates
from ..cl_ast.name import Name
from ..cl_ast.scope import ScopeError


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
                       | STRUCT_ID
                       | TYPENAME_ID
                       | NAMESPACE_ID
                       | METHOD_ID
                       | VARIABLE_ID
                       | TEMPLATE_STRUCT_ID
                       | TEMPLATE_METHOD_ID
                       | TEMPLATE_TYPENAME_ID
    """
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
    if len(p[1]) == 1:
        if p[1][0].type.is_void():
            if p[1][0].name:
                p.lexer.error("argument '%s' may not have void type" % p[1][0].name,
                              p[1][0].position)
            p[0] = []
        else:
            p[0] = p[1]
    else:
        p[0] = p[1]


def p_method_parameters_none(p):
    """
        method_parameters :
    """
    p[0] = []


def create_method_container(lexer, name):
    if name.is_qualified():
        owner = name.parent.target
    else:
        owner = lexer.scopes[-1].scope_owner
    assert owner
    m = methods.Method(lexer, name.position, name.name, owner)
    owner.scope.add(m)
    return m


def p_create_method(p):
    """
        create_method :
    """
    name = p[-1]
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if object_type in ('METHOD_ID', 'TEMPLATE_METHOD_ID',):
            p[0] = name.target
        else:
            if name.is_qualified():
                p.lexer.error('qualified name %s does not name a method' % name,
                                name.position)
            else:
                p.lexer.error('name %s does not name a method' % name,
                                name.position)
            p.lexer.note('previously declared here', name.target.position)
            p[0] = create_method_container(p.lexer, name)
    elif name.target and object_type == 'METHOD_ID':
        p[0] = create_method_container(p.lexer, name)
    else:
        # method not in this scope: redeclare
        p[0] = create_method_container(p.lexer, name)
    p.set_position_absolute(0, name.position)


def p_create_special_method(p):
    """
        create_special_method :
    """
    name = p[-1]
    klass = name.parent and name.parent.target or p.lexer.scopes[-1].scope_owner
    obj = name.target
    if obj:
        p[0] = obj
    else:
        # constructors should take step above
        assert name.name[0] == '~'
        assert klass, 'resolution of %s failed' % name
        assert klass.scope, '%s is not defined' % name
        if name.name[1:] != klass.name:
            p.lexer._error("expected the class name after '~' to name the enclosing class", name.position)
        if klass.scope.destructor:
            p[0] = klass.scope.destructor
        else:
            p[0] = klass.scope.destructor = methods.SpecialMethod(p.lexer, name.position, name.name, klass)


def p_create_op(p):
    """
        create_op :
    """
    name = p[-1]
    m = name.target
    if not m:
        m = create_method_container(p.lexer, name)
    p[0] = m


def p_create_castop(p):
    """
        create_castop :
    """
    name = p[-1]
    cast_type = name.data
    m = name.target
    if m:
        assert len(m.overloads) >= 1
        #try:
        #    m.overloads[0].return_type.distance(cast_type, types.CAST_NONE)
        #except types.CastError:
        #    assert False
    else:
        m = create_method_container(p.lexer, name)
        m.owner.scope.casts.append((cast_type, m))
    p[0] = m


def p_push_overload_scope(p):
    """
        push_overload_scope :
    """
    overload = p[-1]
    overload.push_scope(p.position(-1), methods.OverloadScope(overload, p.position(-1)))
    p[0] = overload


def p_pop_overload_scope(p):
    """
        pop_overload_scope :
    """
    overload = p[-3]
    p.lexer.pop_scope(overload.scope)


def p_method_attribute(p):
    """
        method_attribute : CONST
    """
    p[0] = p[1]


def p_method_attribute_purevirtual(p):
    """
        method_attribute : EQUALS INT_CONST_OCT
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
        method_declaration_prefix : declaration_specifier_list type object_name verify_template_stack_1 create_method
    """
    p[0] = (p[2], p[1], p[5], p[4])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list type operator_overload_name verify_template_stack_1 create_op
    """
    p[0] = (p[2], p[1], p[5], p[4])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_cast_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list operator_cast_name verify_template_stack_1 create_castop
    """
    p[0] = (p[3].data, p[1], p[4], p[3])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_special_method(p):
    """
        method_declaration_prefix : declaration_specifier_list special_method_name verify_template_stack_1 create_special_method
    """
    p[0] = (None, p[1], p[4], p[3])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_ctor_2(p):
    """
        method_declaration_prefix : declaration_specifier_list type_name verify_template_stack_1
    """
    name = p[3]
    klass = name.target
    if not name.is_qualified():
        if name.name != p.lexer.scopes[-1].scope_owner.name:
            p.lexer.error('expected enclosing class name', name.position)
            p[0] = (None, p[1], methods.SpecialMethod(p.lexer, p.position(2), name.name, klass), p[3])
            return
    assert klass, 'resolution of %s failed' % name
    assert klass.scope
    if klass.scope.constructor:
        m = klass.scope.constructor
    else:
        m = klass.scope.constructor = methods.SpecialMethod(p.lexer, p.position(2), name.name, klass)
    p[0] = (None, p[1], m, p[3])
    m.push_scope_recursive(name.position)


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix LPAREN method_parameters RPAREN method_attributes
    """
    if p[1][0]:
        assert isinstance(p[1][0], types.TypeRef), p[1][3]
    m = p[1][2]
    name = p[1][3]
    if name.template_bindings and name.template:
        t = name.template_bindings.template
        t.parent.scope.remove(t)
    else:
        t = None
    try:
        p[0] = m.find_overload(t, p[3], p.position(2), p[1][0], p[1][1] + p[5])
        if not p[0]:
            if p[1][3].is_qualified():
                p.lexer.error("out-of-line definition of '%s' does not match any declaration in %s" % (p[1][3].name, m.parent.scope.scope_owner.pretty_name()),
                            p.position(2))
            p[0] = m.create_overload(t, p[3], p.position(2), p[1][0], p[1][1] + p[5])
    finally:
        p.set_position(0, 2)
        if t:
            t.bind(p[0].template)
        m.pop_scope_recursive()


def p_method_definition(p):
    """
        method_definition : method_declaration push_overload_scope initializer_list_opt statement_block pop_overload_scope
    """

