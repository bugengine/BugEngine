from .. import cl_ast
from ..cl_ast.name import Name


def p_id(p):
    """
        namespace_id :                  NAMESPACE_ID
        namespace_id_shadow :           NAMESPACE_ID_SHADOW
        struct_id :                     STRUCT_ID
        struct_id_shadow :              STRUCT_ID_SHADOW
        typename_id :                   TYPENAME_ID
        typename_id_shadow :            TYPENAME_ID_SHADOW
        method_id :                     METHOD_ID
        method_id_shadow :              METHOD_ID_SHADOW
    """
    p[0] = Name(p.lexer, (p[1],), p.position(1),
                p.slice[1].found_object,
                qualified = not p.slice[1].type.endswith('SHADOW'),
                dependent = False)


def p_id_template(p):
    """
        template_struct_id :            TEMPLATE_STRUCT_ID
        template_struct_id_shadow :     TEMPLATE_STRUCT_ID_SHADOW
        template_typename_id :          TEMPLATE_TYPENAME_ID
        template_typename_id_shadow :   TEMPLATE_TYPENAME_ID_SHADOW
    """
    found_object = p.slice[1].found_object
    if found_object:
        if p.lexer.template_stack:
            template = p.lexer.template_stack.pop()
            if template:
                found_object = found_object.find_instance(template, template.parameters, p.position(1))
                p[0] = Name(p.lexer, (p[1],), p.position(1),
                            found_object,
                            targets=((found_object, template.parameters, template)),
                            qualified = not p.slice[1].type.endswith('SHADOW'),
                            dependent = (p.slice[1].type.find('TYPENAME') != -1
                                    or p.slice[1].found_object and p.slice[1].found_object.templates))
                return
    p[0] = Name(p.lexer, (p[1],), p.position(1),
                found_object,
                qualified = not p.slice[1].type.endswith('SHADOW'),
                dependent = (p.slice[1].type.find('TYPENAME') != -1
                            or p.slice[1].found_object and p.slice[1].found_object.templates))


def p_template_new_template_id(p):
    """
        type_name_tpl_qualified : TEMPLATE ID template_arguments
                                | TEMPLATE NAMESPACE_ID template_arguments
                                | TEMPLATE VARIABLE_ID template_arguments
                                | TEMPLATE METHOD_ID template_arguments
                                | TEMPLATE STRUCT_ID template_arguments
                                | TEMPLATE TYPENAME_ID template_arguments
                                | TEMPLATE TEMPLATE_METHOD_ID template_arguments
                                | TEMPLATE TEMPLATE_STRUCT_ID template_arguments
                                | TEMPLATE TEMPLATE_TYPENAME_ID template_arguments
                                | TEMPLATE NAMESPACE_ID_SHADOW template_arguments
                                | TEMPLATE VARIABLE_ID_SHADOW template_arguments
                                | TEMPLATE METHOD_ID_SHADOW template_arguments
                                | TEMPLATE STRUCT_ID_SHADOW template_arguments
                                | TEMPLATE TYPENAME_ID_SHADOW template_arguments
                                | TEMPLATE TEMPLATE_METHOD_ID_SHADOW template_arguments
                                | TEMPLATE TEMPLATE_STRUCT_ID_SHADOW template_arguments
                                | TEMPLATE TEMPLATE_TYPENAME_ID_SHADOW template_arguments
    """
    p[0] = Name(p.lexer, (p[2],), p.position(2), targets = ((None, p[3], None),),
                dependent=True)


def p_template_id(p):
    """
        struct_id : TEMPLATE_STRUCT_ID template_arguments
        struct_id_shadow : TEMPLATE_STRUCT_ID_SHADOW template_arguments
        typename_id : TEMPLATE_TYPENAME_ID template_arguments
        typename_id_shadow : TEMPLATE_TYPENAME_ID_SHADOW template_arguments
        method_id : TEMPLATE_METHOD_ID template_arguments                             %prec TEMPLATEGT
        method_id_shadow : TEMPLATE_METHOD_ID_SHADOW template_arguments               %prec TEMPLATEGT
    """
    try:
        tpl = None
        if p.lexer.template_stack:
            tpl = p.lexer.template_stack.pop()
            if not tpl:
                p.lexer._error('template specialization or definition requires a template parameter list '
                               'corresponding to the nested type %s<%s>' % (p[1], ', '.join(str(t) for t in p[2])),
                               p.position(1))
        target = p.slice[1].found_object.find_instance(tpl, p[2], p.position(1))
    except cl_ast.templates.Template.InstantiationError as e:
        target = None
        def show_template_error(e):
            if e.inner_error:
                show_template_error(e.inner_error)
                p.lexer._note(e.message, e.position)
            else:
                p.lexer._error(e.message, e.position)
        show_template_error(e)
        p.lexer._note("in instantiation of template '%s' requested here" % p[1], p.position(1))
    p[0] = Name(p.lexer, (p[1],), p.position(1), target,
                targets = ((target, p[2], p.slice[1].found_object),),
                qualified = not p.slice[1].type.endswith('SHADOW'),
                dependent = (target==None))
    p.lexer.set_search_scope_ifn(p.position(1), target)


def p_object_name_namespace(p):
    """
        object_name : namespace_id SCOPE object_name_qualified
                    | namespace_id_shadow SCOPE object_name_qualified
                    | struct_id SCOPE object_name_struct_qualified
                    | struct_id_shadow SCOPE object_name_struct_qualified
                    | typename_id SCOPE object_name_struct_qualified
                    | typename_id_shadow SCOPE object_name_struct_qualified
                    | template_typename_id SCOPE object_name_struct_qualified
                    | template_typename_id_shadow SCOPE object_name_struct_qualified
        type_name : namespace_id SCOPE type_name_qualified
                  | namespace_id_shadow SCOPE type_name_qualified
                  | struct_id SCOPE type_name_struct_qualified
                  | struct_id_shadow SCOPE type_name_struct_qualified
                  | typename_id SCOPE type_name_struct_qualified
                  | typename_id_shadow SCOPE type_name_struct_qualified
                  | template_typename_id SCOPE type_name_struct_qualified
                  | template_typename_id_shadow SCOPE type_name_struct_qualified
        template_name : namespace_id SCOPE template_name_qualified
                      | namespace_id_shadow SCOPE template_name_qualified
                      | struct_id SCOPE template_name_struct_qualified
                      | struct_id_shadow SCOPE template_name_struct_qualified
                      | typename_id SCOPE template_name_struct_qualified
                      | typename_id_shadow SCOPE template_name_struct_qualified
                      | template_typename_id SCOPE template_name_struct_qualified
                      | template_typename_id_shadow SCOPE template_name_struct_qualified
        special_method_name : namespace_id SCOPE special_method_name_qualified
                            | namespace_id_shadow SCOPE special_method_name_qualified
                            | struct_id SCOPE special_method_name_struct_qualified
                            | struct_id_shadow SCOPE special_method_name_struct_qualified
                            | typename_id SCOPE special_method_name_struct_qualified
                            | typename_id_shadow SCOPE special_method_name_struct_qualified
                            | template_typename_id SCOPE special_method_name_struct_qualified
                            | template_typename_id_shadow SCOPE special_method_name_struct_qualified
        cast_method_name : namespace_id SCOPE cast_method_name_qualified
                         | namespace_id_shadow SCOPE cast_method_name_qualified
                         | struct_id SCOPE cast_method_name_struct_qualified
                         | struct_id_shadow SCOPE cast_method_name_struct_qualified
                         | typename_id SCOPE cast_method_name_struct_qualified
                         | typename_id_shadow SCOPE cast_method_name_struct_qualified
                         | template_typename_id SCOPE cast_method_name_struct_qualified
                         | template_typename_id_shadow SCOPE cast_method_name_struct_qualified
    """
    p[0] = p[1] + p[3]


def p_object_name_struct_qualified(p):
    """
        object_name_qualified : namespace_id SCOPE object_name_qualified
                              | object_name_struct_qualified
        object_name_struct_qualified : struct_id SCOPE object_name_struct_qualified
                                     | type_name_tpl_qualified SCOPE object_name_struct_qualified
                                     | object_name_id_qualified

        type_name_qualified : namespace_id SCOPE type_name_qualified
                            | type_name_struct_qualified

        type_name_struct_qualified : struct_id SCOPE type_name_struct_qualified
                                   | type_name_tpl_qualified SCOPE type_name_struct_qualified
                                   | type_name_id_qualified

        template_name_qualified : namespace_id SCOPE template_name_qualified
                                | template_name_struct_qualified

        template_name_struct_qualified : struct_id SCOPE template_name_struct_qualified
                                       | type_name_tpl_qualified SCOPE template_name_struct_qualified
                                       | template_name_id_qualified

        special_method_name_qualified : namespace_id SCOPE special_method_name_qualified
                                      | special_method_name_struct_qualified
        special_method_name_struct_qualified : struct_id SCOPE special_method_name_struct_qualified
                                             | type_name_tpl_qualified SCOPE special_method_name_struct_qualified
                                             | special_method_name_id_qualified

        cast_method_name_qualified : namespace_id SCOPE cast_method_name_qualified
                                   | cast_method_name_struct_qualified
        cast_method_name_struct_qualified : struct_id SCOPE cast_method_name_struct_qualified
                                          | type_name_tpl_qualified SCOPE cast_method_name_struct_qualified
                                          | cast_method_name_id_qualified

    """
    if len(p) > 2:
        p[0] = p[1] + p[3]
    else:
        p[0] = p[1]


def p_object_name_namespace_root(p):
    """
        object_name : SCOPE object_name_qualified
        type_name : SCOPE type_name_qualified
        template_name : SCOPE template_name_qualified
        special_method_name : SCOPE special_method_name_qualified
        cast_method_name : SCOPE cast_method_name_qualified
    """
    p[0] = p[2]
    p[0].absolute = True


def p_object_name(p):
    """
        object_name : VARIABLE_ID
                    | VARIABLE_ID_SHADOW

        object_name_id_qualified : VARIABLE_ID
    """
    p[0] = Name(p.lexer, (p[1],), p.position(1), p.slice[1].found_object,
                qualified = not p.slice[1].type.endswith('SHADOW'))


def p_object_name_template(p):
    """
        object_name : TEMPLATE_METHOD_ID                                            %prec TEMPLATEGT
                    | TEMPLATE_METHOD_ID_SHADOW                                     %prec TEMPLATEGT

        object_name_id_qualified : TEMPLATE_METHOD_ID                               %prec TEMPLATEGT
    """
    found_object = p.slice[1].found_object
    if found_object:
        if p.lexer.template_stack:
            template = p.lexer.template_stack.pop()
            if template:
                template.bind(found_object)
                found_object = found_object.find_specialization(p.position(1), template.parameters)
            else:
                p.lexer._error('template specialization or definition requires a '
                                'template parameter list corresponding to '
                                'the nested type %s' % p[1], p.position(1))
                found_object = found_object.scope.items[0][1]

    p[0] = Name(p.lexer, (p[1],), p.position(1), found_object,
                qualified = not p.slice[1].type.endswith('SHADOW'))


def p_object_name_constructor(p):
    """
        special_method_id :             SPECIAL_METHOD_ID
    """
    owner = p.slice[1].found_object.owner
    if not owner.scope:
        p.lexer._error('invalid use of incomplete type %s' % owner.name, p.position(1))
        p.lexer._info('forward declaration of %s' % owner.name, owner.position)
    p[0] = Name(p.lexer, (p[1],), p.position(1), p.slice[1].found_object,
                qualified = True, data=owner)


def p_object_name_destructor(p):
    """
        special_method_name : NOT STRUCT_ID_SHADOW
                            | NOT TEMPLATE_STRUCT_ID_SHADOW
        special_method_name_id_qualified : NOT STRUCT_ID_SHADOW
                                         | NOT TEMPLATE_STRUCT_ID_SHADOW
    """
    owner = p.slice[2].found_object
    scope = p.lexer.scopes[-1]
    if owner.scope != scope:
        p.lexer._error('declaration of ~%s as a member of %s' % (owner.name, scope.owner.name), p.position(1))
    if not owner.scope:
        p.lexer._error('invalid use of incomplete type %s' % owner.name, p.position(1))
        p.lexer._info('forward declaration of %s' % owner.name, owner.position)
    p[0] = Name(p.lexer, (p[1]+p[2],), p.position(1), owner.scope.destructor,
                qualified = False, data=owner)


def p_object_name_destructor_2(p):
    """
        special_method_name : NOT SPECIAL_METHOD_ID
        special_method_name_id_qualified : NOT SPECIAL_METHOD_ID
    """
    owner = p.slice[2].found_object.owner
    if not owner.scope:
        p.lexer._error('invalid use of incomplete type %s' % owner.name, p.position(1))
        p.lexer._info('forward declaration of %s' % owner.name, owner.position)
    p[0] = Name(p.lexer, (p[1]+p[2],), p.position(1), owner.scope and owner.scope.destructor,
                qualified = True, data=owner)


def p_operator_name(p):
    """
        operator_name : OPERATOR PLUS
                      | OPERATOR MINUS
                      | OPERATOR TIMES
                      | OPERATOR DIVIDE
                      | OPERATOR MOD
                      | OPERATOR OR
                      | OPERATOR AND
                      | OPERATOR NOT
                      | OPERATOR XOR
                      | OPERATOR LSHIFT
                      | OPERATOR RSHIFT
                      | OPERATOR LOR
                      | OPERATOR LAND
                      | OPERATOR LNOT
                      | OPERATOR LT
                      | OPERATOR LE
                      | OPERATOR GT
                      | OPERATOR GE
                      | OPERATOR EQ
                      | OPERATOR NE
                      | OPERATOR EQUALS
                      | OPERATOR TIMESEQUAL
                      | OPERATOR DIVEQUAL
                      | OPERATOR MODEQUAL
                      | OPERATOR PLUSEQUAL
                      | OPERATOR MINUSEQUAL
                      | OPERATOR LSHIFTEQUAL
                      | OPERATOR RSHIFTEQUAL
                      | OPERATOR ANDEQUAL
                      | OPERATOR XOREQUAL
                      | OPERATOR OREQUAL
                      | OPERATOR PLUSPLUS
                      | OPERATOR MINUSMINUS
                      | OPERATOR ARROW
                      | OPERATOR LPAREN RPAREN
                      | OPERATOR LBRACKET RBRACKET
                      | OPERATOR PERIOD
                      | OPERATOR COMMA
    """
    p[0] = ('!op_%s' % p.slice[2].type.lower(), p.slice[2].found_object)
    p.set_position(0, 2)


def p_operator_cast(p):
    """
        operator_cast : OPERATOR type
    """
    p[0] = Name(p.lexer, ('!cast<%s>'%p[2].signature(),), p.position(1), None,
                qualified = False,
                dependent = False,
                data = p[2])
    p.set_position(0, 1)


def p_object_name_id(p):
    """
        object_name : ID
        object_name_id_qualified : ID
        object_name_id_qualified : METHOD_ID_SHADOW
        object_name_id_qualified : VARIABLE_ID_SHADOW
        object_name_id_qualified : STRUCT_ID_SHADOW
        object_name_id_qualified : TEMPLATE_METHOD_ID_SHADOW
        object_name_id_qualified : TEMPLATE_STRUCT_ID_SHADOW
    """
    p[0] = Name(p.lexer, (p[1],), p.position(1))


def p_object_name_operator(p):
    """
        object_name : operator_name
        object_name_id_qualified : operator_name
    """
    p[0] = Name(p.lexer, (p[1][0],), p.position(1), p[1][1],
                qualified = p[1][1] != None,
                dependent = False)


def p_type_name(p):
    """
        type_name : struct_id                                                           %prec NAME0
                  | typename_id                                                         %prec NAME0

        template_name : template_struct_id
                      | template_typename_id                                            %prec NAME0

        type_name_id_qualified : struct_id                                              %prec NAME0
                               | typename_id

        template_name_id_qualified : template_struct_id
                                   | template_typename_id

        special_method_name : special_method_id
        special_method_name_id_qualified : special_method_id

        object_name : method_id
                    | method_id_shadow
        object_name_id_qualified : method_id
                                 | type_name_tpl_qualified                              %prec NAME2
    """
    p[0] = p[1]


def p_cast_name(p):
    """
        cast_method_name : operator_cast
        cast_method_name_id_qualified : operator_cast
    """
    p[0] = p[1]


def p_type_name_shadow(p):
    """
        type_name : struct_id_shadow                                                    %prec NAME0
                  | typename_id_shadow                                                  %prec NAME0
        template_name : template_struct_id_shadow
                      | template_typename_id_shadow                                     %prec NAME0
    """
    p[0] = p[1]
