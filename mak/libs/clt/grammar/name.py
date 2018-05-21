def p_id(p):
    """
        namespace_id : NAMESPACE_ID
        namespace_id_shadow : NAMESPACE_ID_SHADOW
        struct_id : STRUCT_ID
        struct_id_shadow : STRUCT_ID_SHADOW
        typename_id : TYPENAME_ID
        typename_id_shadow : TYPENAME_ID_SHADOW
        template_struct_id : TEMPLATE_STRUCT_ID
        template_struct_id_shadow : TEMPLATE_STRUCT_ID_SHADOW
        template_typename_id : TEMPLATE_TYPENAME_ID
        template_typename_id_shadow : TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = ((p[1],), not p.slice[1].type.endswith('SHADOW'), p.slice[1].found_object)
    p.set_position(0, 1)


def p_template_struct_id(p):
    """
        struct_id : TEMPLATE_STRUCT_ID template_arguments
        typename_id : TEMPLATE_TYPENAME_ID template_arguments
        struct_id_shadow : TEMPLATE_STRUCT_ID_SHADOW template_arguments
        typename_id_shadow : TEMPLATE_TYPENAME_ID_SHADOW template_arguments
    """
    p[0] = ((p[1],), False, None) # TODO
    p.set_position(0, 1)
    p.lexer.set_search_scope_ifn(p.slice[1].found_object.create_instance(p[2]))


def p_object_name_namespace(p):
    """
        object_name : namespace_id SCOPE object_name_qualified
                    | namespace_id_shadow SCOPE object_name_qualified
                    | struct_id SCOPE object_name_struct_qualified
                    | struct_id_shadow SCOPE object_name_struct_qualified
        type_name : namespace_id SCOPE type_name_qualified
                  | namespace_id_shadow SCOPE type_name_qualified
                  | struct_id SCOPE type_name_struct_qualified
                  | struct_id_shadow SCOPE type_name_struct_qualified
    """
    p[0] = ((p[1][0],) + p[3][0], True, p[3][2])


def p_object_name_struct_qualified(p):
    """
        object_name_qualified : namespace_id SCOPE object_name_qualified
                              | object_name_struct_qualified
        object_name_struct_qualified : struct_id SCOPE object_name_struct_qualified
                                     | object_name_id_qualified

        type_name_qualified : namespace_id SCOPE type_name_qualified
                            | type_name_struct_qualified

        type_name_struct_qualified : struct_id SCOPE type_name_struct_qualified
                                   | type_name_id_qualified
    """
    if len(p) > 2:
        p[0] = ((p[1][0],) + p[3][0], True, p[3][2])
    else:
        p[0] = p[1]


def p_object_name_namespace_root(p):
    """
        object_name : SCOPE object_name_qualified
        type_name : SCOPE type_name_qualified
    """
    p[0] = ((None,) + p[2][0], True, p[2][2])


def p_object_name(p):
    """
        object_name : METHOD_ID                                                         %prec PRIO0
                    | VARIABLE_ID                                                       %prec PRIO0

        object_name_id_qualified : METHOD_ID                                            %prec PRIO0
                                 | VARIABLE_ID                                          %prec PRIO0
    """
    p[0] = ((p[1],), True, p.slice[1].found_object)
    p.set_position(0, 1)


def p_object_name_shadow(p):
    """
        object_name : METHOD_ID_SHADOW                                                  %prec PRIO0
                    | VARIABLE_ID_SHADOW                                                %prec PRIO0
    """
    p[0] = ((p[1],), False, p.slice[1].found_object)
    p.set_position(0, 1)


def p_object_name_id(p):
    """
        object_name : ID                                                                %prec PRIO0
    """
    p[0] = ((p[1],), False, None)
    p.set_position(0, 1)


def p_type_name(p):
    """
        type_name : struct_id                                                           %prec PRIO0
                  | typename_id                                                         %prec PRIO0
                  | template_struct_id                                                  %prec PRIO0
                  | template_typename_id                                                %prec PRIO0

        type_name_id_qualified : struct_id                                              %prec PRIO0
                               | typename_id                                            %prec PRIO0
                               | template_struct_id                                     %prec PRIO0
                               | template_typename_id                                   %prec PRIO0
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_type_name_shadow(p):
    """
        type_name : struct_id_shadow                                                    %prec PRIO0
                  | typename_id_shadow                                                  %prec PRIO0
                  | template_struct_id_shadow                                           %prec PRIO0
                  | template_typename_id_shadow                                         %prec PRIO0
    """
    p[0] = p[1]
    p.set_position(0, 1)

