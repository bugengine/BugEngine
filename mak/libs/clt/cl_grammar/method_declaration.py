from ..cl_ast import Name, Method, SpecialMethod, Parameter, TypeRef
from be_typing import cast, TYPE_CHECKING


def p_parameter_name_none(p):
    # type: (YaccProduction) -> None
    """
        parameter_name :
    """
    p[0] = None


def p_parameter_name(p):
    # type: (YaccProduction) -> None
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


def p_parameter_initial_value_none(p):
    # type: (YaccProduction) -> None
    """
        parameter_initial_value_opt :
    """
    pass


def p_parameter_initial_value(p):
    # type: (YaccProduction) -> None
    """
        parameter_initial_value_opt : EQUALS expression
    """
    p[0] = p[2]


def p_method_parameter_list_last(p):
    # type: (YaccProduction) -> None
    """
        method_parameter_list : type parameter_name parameter_initial_value_opt
    """
    p[0] = [Parameter(p.lexer, p[1].position, p[2], p[1], p[3])]


def p_method_parameter_list(p):
    # type: (YaccProduction) -> None
    """
        method_parameter_list : type parameter_name parameter_initial_value_opt COMMA method_parameter_list
    """
    p[0] = [Parameter(p.lexer, p[1].position, p[2], p[1], p[3])] + p[5]


def p_method_parameters(p):
    # type: (YaccProduction) -> None
    """
        method_parameters : method_parameter_list
    """
    if len(p[1]) == 1:
        if p[1][0].type.is_void():
            if p[1][0].name:
                p.lexer.logger.C0202(p[1][0].position, p[1][0].name)
            p[0] = []
        else:
            p[0] = p[1]
    else:
        p[0] = p[1]


def p_method_parameters_none(p):
    # type: (YaccProduction) -> None
    """
        method_parameters :
    """
    p[0] = []


def create_method_container(lexer, name):
    # type: (ClLexer, Name) -> Method
    if name.is_qualified():
        assert name.parent
        owner = name.parent.target
    else:
        owner = lexer.scopes[-1].scope_owner
    assert owner
    m = Method(lexer, name.position, name.name, owner)
    owner.scope.add(m)
    return m


def find_or_create_method(p, name, owner):
    # type: (YaccProduction, Name, CppObject) -> Method
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        assert name.target is not None
        if object_type in (
            'METHOD_ID',
            'TEMPLATE_METHOD_ID',
        ):
            assert isinstance(name.target, Method)
            return name.target
        else:
            if name.is_qualified():
                p.lexer.logger.C0112(name.position, str(name))
            else:
                p.lexer.logger.C0113(name.position, str(name))
            p.lexer.logger.I0000(name.target.position)
            return create_method_container(p.lexer, name)
    elif name.target and object_type == 'METHOD_ID':
        return create_method_container(p.lexer, name)
    else:
        # method not in this scope: redeclare
        return create_method_container(p.lexer, name)


def p_create_special_method(p):
    # type: (YaccProduction) -> None
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
            p.lexer.logger.C0203(name.position)
        scope = cast('StructScope', klass.scope)
        if scope.destructor:
            p[0] = scope.destructor
        else:
            p[0] = scope.destructor = SpecialMethod(p.lexer, name.position, name.name, klass)


def p_create_op(p):
    # type: (YaccProduction) -> None
    """
        create_op :
    """
    name = p[-1]
    m = name.target
    if not m:
        m = create_method_container(p.lexer, name)
    p[0] = m


def p_create_castop(p):
    # type: (YaccProduction) -> None
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
    # type: (YaccProduction) -> None
    """
        push_overload_scope :
    """
    overload = p[-1]
    overload.define(p.position(-1))
    p[0] = overload


def p_pop_overload_scope(p):
    # type: (YaccProduction) -> None
    """
        pop_overload_scope :
    """
    overload = p[-3]
    overload.pop_scope_recursive()


def p_method_attribute(p):
    # type: (YaccProduction) -> None
    """
        method_attribute : CONST
    """
    p[0] = p[1]


def p_method_attribute_purevirtual(p):
    # type: (YaccProduction) -> None
    """
        method_attribute : EQUALS INT_CONST_OCT
    """
    p[0] = p[1]


def p_method_attributes(p):
    # type: (YaccProduction) -> None
    """
        method_attributes :
                          | method_attribute method_attributes
    """
    if len(p) > 2:
        p[0] = [p[1]] + p[2]
    else:
        p[0] = []


def p_initializer(p):
    # type: (YaccProduction) -> None
    """
        initializer : object_name LPAREN expression_list_opt RPAREN
    """


def p_initializer_list(p):
    # type: (YaccProduction) -> None
    """
        initializer_list : initializer
                         | initializer COMMA initializer_list
    """


def p_initializer_list_opt(p):
    # type: (YaccProduction) -> None
    """
        initializer_list_opt : COLON initializer_list
                             |
    """


def p_object_scope(p):
    # type: (YaccProduction) -> None
    """
        push_object_scope :
    """
    name = p[-2]
    if name.is_qualified():
        owner = name.parent.target
    else:
        owner = p.lexer.scopes[-1].scope_owner
    owner.push_scope_recursive(name.position)
    p[0] = owner


def p_object_declaration(p):
    # type: (YaccProduction) -> None
    """
        object_declaration : declaration_specifier_list type object_name verify_template_stack_1_opt variable_array_specifier_opt push_object_scope
                           | declaration_specifier_list type_definition object_name verify_template_stack_1_opt variable_array_specifier_opt push_object_scope
    """
    p[0] = (p[1], p[2], p[4], p[6], p[5])


def p_method_declaration_prefix_operator(p):
    # type: (YaccProduction) -> None
    """
        method_declaration_prefix : declaration_specifier_list type operator_overload_name verify_template_stack_1 create_op
    """
    p[0] = (p[2], p[1], p[5], p[4])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_cast_operator(p):
    # type: (YaccProduction) -> None
    """
        method_declaration_prefix : declaration_specifier_list operator_cast_name verify_template_stack_1 create_castop
    """
    p[0] = (p[3].data, p[1], p[4], p[3])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_special_method(p):
    # type: (YaccProduction) -> None
    """
        method_declaration_prefix : declaration_specifier_list special_method_name verify_template_stack_1 create_special_method
    """
    p[0] = (None, p[1], p[4], p[3])
    p[0][2].push_scope_recursive(p[0][2].position)


def p_method_declaration_prefix_ctor_2(p):
    # type: (YaccProduction) -> None
    """
        method_declaration_prefix : declaration_specifier_list type_name verify_template_stack_1
    """
    name = p[3]
    klass = name.target
    if not name.is_qualified():
        if name.name != p.lexer.scopes[-1].scope_owner.name:
            p.lexer.logger.C0204(name.position)
            p[0] = (None, p[1], SpecialMethod(p.lexer, p.position(2), name.name, klass), p[3])
            return
    assert klass, 'resolution of %s failed' % name
    assert klass.scope
    if klass.scope.constructor:
        m = klass.scope.constructor
    else:
        m = klass.scope.constructor = SpecialMethod(p.lexer, p.position(2), name.name, klass)
    p[0] = (None, p[1], m, p[3])
    m.push_scope_recursive(name.position)


def p_method_declaration(p):
    # type: (YaccProduction) -> None
    """
        method_declaration : method_declaration_prefix LPAREN method_parameters RPAREN method_attributes
    """
    if p[1][0]:
        assert isinstance(p[1][0], TypeRef), p[1][3]
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
                p.lexer.logger.C0205(p.position(2), p[1][3].name, m.owner.pretty_name())
            p[0] = m.create_overload(t, p[3], p.position(2), p[1][0], p[1][1] + p[5])
    finally:
        p.set_position(0, 2)
        if t and p[0]:
            t.bind(p[0].template)
        m.pop_scope_recursive()


def p_method_declaration_from_object(p):
    # type: (YaccProduction) -> None
    """
        method_declaration : object_declaration LPAREN method_parameters RPAREN method_attributes
    """
    attributes, return_type, name, owner, array_specifier = p[1]
    owner.pop_scope_recursive()
    if id(array_specifier) != id(return_type):
        p.lexer.logger.C0206(array_specifier.position, name)
    assert isinstance(return_type, TypeRef), return_type
    m = find_or_create_method(p, name, owner)
    if name.template_bindings and name.template:
        t = name.template_bindings.template
        t.parent.scope.remove(t)
    else:
        t = None
    try:
        p[0] = m.find_overload(t, p[3], p.position(2), return_type, attributes + p[5])
        if not p[0]:
            if name.is_qualified():
                p.lexer.logger.C0205(p.position(2), name, owner.pretty_name())
            p[0] = m.create_overload(t, p[3], p.position(2), return_type, attributes + p[5])
    finally:
        p.set_position(0, 2)
        if t and p[0]:
            t.bind(p[0].template)


def p_method_definition(p):
    # type: (YaccProduction) -> None
    """
        method_definition : method_declaration push_overload_scope initializer_list_opt statement_block pop_overload_scope
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ..cl_lexer import ClLexer
    from ..cl_ast.cppobject import CppObject
    from ..cl_ast.ast_types.struct import StructScope