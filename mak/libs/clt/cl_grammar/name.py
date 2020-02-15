from ..cl_ast import Name, CppError, CastOptions, CastError, DependentName
from ..cl_ast.ast_types import Struct
from ..cl_ast.ast_templates import Template, BaseTemplateObject, TemplateTemplateParameter
from be_typing import cast, TYPE_CHECKING
from ply.lex import LexToken


def p_qualifier_empty(p):
    # type: (YaccProduction) -> None
    """
        qualifier_opt :                                                                 %prec NAME0
    """
    p[0] = (None, None)
    p.lexer.clear_search_scope()


def p_qualifier_notempty(p):
    # type: (YaccProduction) -> None
    """
        qualifier_opt : qualifier
    """
    p[0] = p[1]


def p_push_root_namespace(p):
    # type: (YaccProduction) -> None
    """
        push_root_namespace :
    """
    p.lexer.set_search_scope(p.lexer.scopes[0].owner)


def p_qualifier_root(p):
    # type: (YaccProduction) -> None
    """
        qualifier : SCOPE push_root_namespace SCOPE_MARKER                          %prec NAME0
    """
    p[0] = (Name(p.lexer, '', p.position(1),
                 p.lexer.scopes[0].owner), Name(p.lexer, '', p.position(1), p.lexer.scopes[0].owner))


def p_name_object(p):
    # type: (YaccProduction) -> None
    """
        namespace_id : NAMESPACE_ID
        namespace_id_shadow : NAMESPACE_ID_SHADOW
        struct_id : STRUCT_ID                                           %prec TPL_REDUCE
        struct_id_shadow : STRUCT_ID_SHADOW                             %prec TPL_REDUCE
        typename_id : TYPENAME_ID                                       %prec TPL_REDUCE
        typename_id_shadow : TYPENAME_ID_SHADOW                         %prec TPL_REDUCE
        method_id : METHOD_ID
        method_id_shadow : METHOD_ID_SHADOW
        variable_id : VARIABLE_ID
        variable_id_shadow : VARIABLE_ID_SHADOW
        special_method_id : SPECIAL_METHOD_ID
        unknown_id : ID
    """
    parent = p[-1]
    assert isinstance(p.slice[1], LexToken)
    is_shadow = not parent[0] and p.slice[1].type.endswith('_SHADOW')
    p[0] = (Name(p.lexer, p[1], p.position(1), parent=parent[0], target=p.slice[1].found_object, shadow=is_shadow),
            Name(p.lexer, p[1], p.position(1), parent=parent[1], target=p.slice[1].found_object, shadow=is_shadow))
    if not p[0][0].target and parent[0]:
        p[0][0].target = DependentName(p.lexer, p.position(1), p[0][0])
        p[0][1].target = DependentName(p.lexer, p.position(1), p[0][1])


def p_name_template_method(p):
    # type: (YaccProduction) -> None
    """
        method_id : TEMPLATE_METHOD_ID template_arguments
        method_id_shadow : TEMPLATE_METHOD_ID_SHADOW template_arguments
    """
    parent = p[-1]
    assert isinstance(p.slice[1], LexToken)
    is_shadow = not parent[0] and p.slice[1].type.endswith('_SHADOW')
    p[0] = (Name(p.lexer, p[1], p.position(1), parent=parent[0], target=p.slice[1].found_object, arguments=p[2], shadow=is_shadow),
            Name(p.lexer, p[1], p.position(1), parent=parent[1], target=p.slice[1].found_object, arguments=p[2], shadow=is_shadow))


def p_name_template_method_deduced(p):
    # type: (YaccProduction) -> None
    """
        template_method_id : TEMPLATE_METHOD_ID                         %prec TPL_REDUCE
        template_method_id_shadow : TEMPLATE_METHOD_ID_SHADOW           %prec TPL_REDUCE
    """
    parent = p[-1]
    assert isinstance(p.slice[1], LexToken)
    is_shadow = not parent[0] and p.slice[1].type.endswith('_SHADOW')
    p[0] = (Name(p.lexer, p[1], p.position(1), parent=parent[0], target=p.slice[1].found_object, shadow=is_shadow),
            Name(p.lexer, p[1], p.position(1), parent=parent[1], target=p.slice[1].found_object, shadow=is_shadow))


def p_name_template_object(p):
    # type: (YaccProduction) -> None
    """
        template_typename_id : TEMPLATE_TYPENAME_ID                     %prec TPL_REDUCE
        template_typename_id_shadow : TEMPLATE_TYPENAME_ID_SHADOW       %prec TPL_REDUCE
        template_struct_id : TEMPLATE_STRUCT_ID                         %prec TPL_REDUCE
        template_struct_id_shadow : TEMPLATE_STRUCT_ID_SHADOW           %prec TPL_REDUCE
    """
    parent = p[-1]
    assert isinstance(p.slice[1], LexToken)
    template = p.slice[1].found_object
    assert isinstance(template, BaseTemplateObject)
    is_shadow = not parent[0] and p.slice[1].type.endswith('_SHADOW')
    p[0] = (Name(p.lexer, p[1], p.position(1), parent=parent[0], target=template, template=template, shadow=is_shadow),
            Name(p.lexer, p[1], p.position(1), parent=parent[1], target=template, template=template, shadow=is_shadow))


def p_push_object(p):
    # type: (YaccProduction) -> None
    """
        push_object  :
    """
    p.lexer.set_search_scope(p[-2][1].target or p[-2][0].target)


def p_template_object(p):
    # type: (YaccProduction) -> None
    """
        template_type : TEMPLATE_STRUCT_ID template_arguments
                      | TEMPLATE_STRUCT_ID_SHADOW template_arguments
                      | TEMPLATE_TYPENAME_ID template_arguments
                      | TEMPLATE_TYPENAME_ID_SHADOW template_arguments
    """
    assert isinstance(p.slice[1], LexToken)
    template = p.slice[1].found_object
    assert isinstance(template, BaseTemplateObject), template
    parent = p[-1]
    template_stack = p.lexer.template_stack
    arguments = p[2]
    errors = [] # type: List[Tuple[Callable[..., Dict[str, Any]], Dict[str, Any]]]
    is_shadow = not p[1][0] and p.slice[1].type.endswith('_SHADOW')
    n = Name(p.lexer, p[1], p.position(1), parent=parent[0], target=None, template=template, arguments=arguments)
    if template_stack:
        bindings = template_stack.bind(template, parent[1] and parent[1].template_bindings or None)
        if not bindings:
            errors.append((p.lexer.logger.C0402, {}))
        try:
            specialization = template.find_instance(bindings and bindings.parameter_binds or {}, arguments, p.position(1))
        except Template.InstantiationError as e:
            p.lexer.log_cpperror(e)
            specialization = None
        if specialization:
            n.target = specialization
        else:
            n.dependent = True
            n.target = DependentName(p.lexer, p.position(1), n)
    else:
        try:
            template_instance = template.instantiate(arguments, p.position(1))
        except Template.InstantiationError as e:
            p.lexer.log_cpperror(e)
            template_instance = DependentName(p.lexer, p.position(1), n)
            n.dependent = True
        else:
            if not template_instance:
                template_instance = DependentName(p.lexer, p.position(1), n)
                n.dependent = True
        n.target = template_instance
        specialization = template_instance
        bindings = None
    p[0] = (n,
            Name(p.lexer,
                 p[1],
                 p.position(1),
                 parent=parent[1],
                 target=specialization,
                 template=template,
                 arguments=arguments,
                 template_bindings=bindings,
                 shadow=is_shadow,
                 errors=errors))


def p_template_object_error(p):
    # type: (YaccProduction) -> None
    """
        template_type : STRUCT_ID template_arguments
                      | STRUCT_ID_SHADOW template_arguments
                      | TYPENAME_ID template_arguments
                      | TYPENAME_ID_SHADOW template_arguments
    """
    p.lexer.logger.C0104(p.position(1), p[1])
    parent = p[-1]
    assert isinstance(p.slice[1], LexToken)
    is_shadow = not parent[0] and p.slice[1].type.endswith('_SHADOW')
    p[0] = (Name(p.lexer, p[1], p.position(1), parent=parent[0], target=p.slice[1].found_object, shadow=is_shadow),
            Name(p.lexer, p[1], p.position(1), parent=parent[1], target=p.slice[1].found_object, shadow=is_shadow))


def p_template_undefined(p):
    # type: (YaccProduction) -> None
    """
        template_type_undefined : TEMPLATE ID template_arguments
    """
    parent = p[-1]
    n = Name(p.lexer, p[2], p.position(2), arguments=p[3], parent=parent[0])
    n.dependent = True
    n.target = DependentName(p.lexer, p.position(2), n)
    template_stack = p.lexer.template_stack
    bindings = template_stack and template_stack.bind(None, parent[1] and parent[1].template_bindings or None) or None
    n2 = Name(p.lexer,
              p[1],
              p.position(1),
              parent=parent[1],
              template=bindings and bindings.template,
              arguments=p[3],
              template_bindings=bindings,
              errors=[(p.lexer.logger.C0115, {})])
    n2.target = DependentName(p.lexer, p.position(1), n2)
    p[0] = (n, n2)


def p_qualifier(p):
    # type: (YaccProduction) -> None
    """
        qualifier : qualifier_opt namespace_id SCOPE push_object SCOPE_MARKER
                  | qualifier_opt namespace_id_shadow SCOPE push_object SCOPE_MARKER
                  | qualifier_opt struct_id SCOPE push_object SCOPE_MARKER
                  | qualifier_opt struct_id_shadow SCOPE push_object SCOPE_MARKER
                  | qualifier_opt typename_id SCOPE push_object SCOPE_MARKER
                  | qualifier_opt typename_id_shadow SCOPE push_object SCOPE_MARKER
                  | qualifier_opt template_type SCOPE push_object SCOPE_MARKER
                  | qualifier template_type_undefined SCOPE push_object SCOPE_MARKER
    """
    p[0] = p[2]


def p_qualifier_template(p):
    # type: (YaccProduction) -> None
    """
        qualifier : qualifier_opt template_struct_id SCOPE push_object SCOPE_MARKER
                  | qualifier_opt template_struct_id_shadow SCOPE push_object SCOPE_MARKER
                  | qualifier_opt template_typename_id SCOPE push_object SCOPE_MARKER
                  | qualifier_opt template_typename_id_shadow SCOPE push_object SCOPE_MARKER
    """
    p[0] = p[2]
    template_stack = p.lexer.template_stack
    bindings = template_stack and template_stack.bind(p[0][1].template, p[0][1] and p[0][1].template_bindings)
    p[0][0].target = p[0][0].target.scope[0][1]
    p[0][1].target = p[0][1].target.scope[0][1]
    p[0][0].template_bindings = bindings
    p[0][1].template_bindings = bindings
    p.lexer.logger.C0105(p[0][0].position, p[0][1])


def p_name_end_template(p):
    # type: (YaccProduction) -> None
    """
        type_name : qualifier_opt template_type          %prec NAME0
    """
    p[0] = p[2]
    p.lexer.clear_search_scope()


def p_name_end_tpl_error(p):
    # type: (YaccProduction) -> None
    """
        template_name : qualifier_opt template_struct_id                                    %prec NAME0
                      | qualifier_opt template_struct_id_shadow                             %prec NAME0
        template_name : qualifier_opt template_typename_id                                  %prec NAME0
                      | qualifier_opt template_typename_id_shadow                           %prec NAME0
    """
    p[0] = p[2]
    name = p[2][1]
    assert isinstance(name.target, BaseTemplateObject)
    template_stack = p.lexer.template_stack
    if template_stack:
        bindings = template_stack.bind(p[2][1].target, p[1][1] and p[1][1].template_bindings)
        if bindings:
            name.template_bindings = bindings
            name.data = name.target.find_instance(bindings.parameter_binds, bindings.template.parameters, name.position)
            if not name.data:
                arg_list = ', '.join(str(p) for p in name.template.parameters)
                p.lexer.logger.C0107(name.position, name, arg_list)
        elif not name.is_shadow:
            p.lexer.logger.C0105(name.position, name)
    # locate more precise definition for templated objects if the template is on the stack
    if not name.data and name.is_shadow:
        for scope in p.lexer.scopes[::-1]:
            if scope.owner.name == name.name:
                o = scope.owner.parent
                t = name.target
                siblings = [id(x) for x in getattr(t, 'siblings', [])]
                if id(o) == id(t) or id(o) in siblings:
                    name.data = scope.owner
                    assert isinstance(name.data, Struct)
                    break
    p.lexer.clear_search_scope()


def p_name_end(p):
    # type: (YaccProduction) -> None
    """
        namespace_name : qualifier_opt namespace_id                     %prec NAME0
                       | qualifier_opt namespace_id_shadow              %prec NAME0
        type_name : qualifier_opt struct_id_shadow                      %prec NAME0
                  | qualifier_opt typename_id_shadow                    %prec NAME0
        type_name : qualifier_opt struct_id                             %prec NAME0
                  | qualifier_opt typename_id                           %prec NAME0
        dependent_scope_name : qualifier template_type_undefined        %prec NAME0
        object_name : qualifier_opt method_id
                    | qualifier_opt variable_id
        object_name : qualifier_opt method_id_shadow
                    | qualifier_opt variable_id_shadow
        special_method_name : qualifier_opt special_method_id
        object_name : qualifier_opt unknown_id
                    | qualifier_opt template_method_id                  %prec NAME0
                    | qualifier_opt template_method_id_shadow           %prec NAME0
    """
    p[0] = p[2]
    template_stack = p.lexer.template_stack
    if template_stack and p[0][1]:
        bindings = template_stack.bind(None, p[0][1] and p[0][1].template_bindings)
        if bindings:
            p[0][1].template = bindings.template
            p[0][1].template_bindings = bindings
        bindings = template_stack.bind(None, p[0][1] and p[0][1].template_bindings)
        if bindings:
            p[0][1].errors.append(
                'extraneous template parameter list in template specialization or out-of-line template definition')
    elif template_stack:
        # error
        pass
    p.lexer.clear_search_scope()


def p_overload_operator(p):
    # type: (YaccProduction) -> None
    """
        overload_operator : PLUS
                          | MINUS
                          | TIMES
                          | DIVIDE
                          | MOD
                          | OR
                          | AND
                          | NOT
                          | XOR
                          | LSHIFT
                          | RSHIFT
                          | LOR
                          | LAND
                          | LNOT
                          | LT
                          | LE
                          | GT
                          | GE
                          | EQ
                          | NE
                          | EQUALS
                          | TIMESEQUAL
                          | DIVEQUAL
                          | MODEQUAL
                          | PLUSEQUAL
                          | MINUSEQUAL
                          | LSHIFTEQUAL
                          | RSHIFTEQUAL
                          | ANDEQUAL
                          | XOREQUAL
                          | OREQUAL
                          | PLUSPLUS
                          | MINUSMINUS
                          | ARROW
                          | LPAREN RPAREN
                          | LBRACKET RBRACKET
                          | COMMA
    """
    p[0] = p[1]


def p_object_name_operator_cast(p):
    # type: (YaccProduction) -> None
    """
        operator_cast_name :    qualifier_opt OPERATOR disable_name_tpl_stack type enable_name_tpl_stack
    """
    template_stack = p.lexer.template_stack
    # casts can be templatized too, so first bind the template parameters, and check for a template cast
    template = None
    template_bindings = None
    value = None
    cast_type = p[4]
    if template_stack:
        if p[1][1]:
            owner = p[1][1].target
            assert owner
            assert owner.scope
            p[1][1].bind()
            template_bindings_orig = p[1][1].template_bindings
        else:
            owner = p.lexer.scopes[-1].scope_owner
            template_bindings_orig = None
        for dest_type, cast_method in getattr(owner.scope, 'casts', []):
            if cast_method.scope.scope_owner != owner:
                t = cast_method.parent
            else:
                t = None
            template_bindings = template_stack.bind(t, template_bindings_orig)
            try:
                d = cast_type.distance(
                    dest_type,
                    CastOptions(CastOptions.CAST_NONE,
                                template_bindings=template_bindings and template_bindings.parameter_binds or {}))
            except CastError:
                pass
            else:
                if d.exact_match():
                    if template_bindings:
                        template = t
                    value = cast_method
                    break
        else:
            if not p[1][1]:
                template_bindings = template_stack.bind_last(None, None)
                template = template_bindings and template_bindings.template
    if value:
        name = value.name
    else:
        name = '!cast_%s' % cast_type.signature(template_bindings and template_bindings.parameter_binds or {})
    p.lexer.clear_search_scope()
    p[0] = (Name(p.lexer,
                 name,
                 p.position(2),
                 target=value,
                 parent=p[1][0],
                 data=cast_type,
                 template=template,
                 template_bindings=template_bindings),
            Name(p.lexer,
                 name,
                 p.position(2),
                 target=value,
                 parent=p[1][1],
                 data=cast_type,
                 template=template,
                 template_bindings=template_bindings))


def p_object_name_operator_overload(p):
    # type: (YaccProduction) -> None
    """
        operator_overload_name :    qualifier_opt OPERATOR overload_operator
    """
    name = '!op_%s' % p[3]
    p.lexer.clear_search_scope()
    if p[1][1]:
        try:
            value = p[1][1].target.scope.find(name, p.position(2), None, True)
        except CppError as e:
            p.lexer.log_cpperror(e)
    else:
        value = p.lexer.lookup_by_name(name, p.position(2))[1]
    p[0] = (Name(p.lexer, name, p.position(2), target=value,
                 parent=p[1][0]), Name(p.lexer, name, p.position(2), target=value, parent=p[1][1]))


def p_object_name_destructor(p):
    # type: (YaccProduction) -> None
    """
        special_method_name : qualifier_opt NOT SPECIAL_METHOD_ID
        special_method_name : qualifier_opt NOT STRUCT_ID_SHADOW
        special_method_name : qualifier_opt NOT TEMPLATE_STRUCT_ID_SHADOW
        special_method_name : qualifier_opt NOT TYPENAME_ID_SHADOW
        special_method_name : qualifier_opt NOT TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = (Name(p.lexer, '~' + p[3], p.position(2),
                 parent=p[1][0]), Name(p.lexer, '~' + p[3], p.position(2), parent=p[1][1]))
    p.lexer.clear_search_scope()


def p_type_name_typename_error(p):
    # type: (YaccProduction) -> None
    """
        type_name_dependent : dependent_scope_name                                  %prec NAME2
    """
    n = p[1][0]
    if p.lexer.template_stack:
        p.lexer.logger.C0106(n.position, n)
    p[0] = (n, None)


def p_type_name_template_name(p):
    # type: (YaccProduction) -> None
    """
        type_name : template_name
    """
    p[0] = p[1]
    n = p[1][1]
    if n.data:
        assert isinstance(n.data, Struct)
        tpl = n.target
        p[0][0].target = n.data
        p[0][1].target = n.data
        p[0][0].data = tpl
        p[0][1].data = tpl
    elif isinstance(p[0][0].target, Template):
        if not p[0][0].is_shadow():
            p.lexer.logger.C0105(n.position, n)
            p.lexer.logger.I0004(p[0][0].target.position)
            p[0][0].data = p[0][0].target
            p[0][1].data = p[0][1].target
            p[0][0].target = None
            p[0][1].target = None


def p_type_name_typename(p):
    # type: (YaccProduction) -> None
    """
        type_name_dependent : TYPENAME dependent_scope_name
                            | TYPENAME object_name
                            | TYPENAME type_name
    """
    n = p[2][0]
    if not n.dependent:
        n.dependent = True
        n.target = DependentName(p.lexer, n.position, n)
    elif not n.target:
        n.target = DependentName(p.lexer, n.position, n)
    p[0] = (n, None)

if TYPE_CHECKING:
    from typing import Any, Callable, Dict, List, Tuple
    from ply.yacc import YaccProduction