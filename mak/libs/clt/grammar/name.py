def p_object_name_namespace(p):
    """
        object_name : NAMESPACE_ID SCOPE object_name_qualified
                    | NAMESPACE_ID_SHADOW SCOPE object_name_qualified
                    | STRUCT_ID SCOPE object_name_struct_qualified
                    | STRUCT_ID_SHADOW SCOPE object_name_struct_qualified
        type_name : NAMESPACE_ID SCOPE type_name_qualified
                  | NAMESPACE_ID_SHADOW SCOPE type_name_qualified
                  | STRUCT_ID SCOPE type_name_struct_qualified
                  | STRUCT_ID_SHADOW SCOPE type_name_struct_qualified
    """
    p[0] = ((p[1],) + p[3][1], True, p[3][2])


def p_object_name_struct_qualified(p):
    """
        object_name_qualified : NAMESPACE_ID SCOPE object_name_qualified
                              | object_name_struct_qualified
        object_name_struct_qualified : STRUCT_ID SCOPE object_name_struct_qualified
                                     | object_name_id_qualified

        type_name_qualified : NAMESPACE_ID SCOPE type_name_qualified
                            | type_name_struct_qualified

        type_name_struct_qualified : STRUCT_ID SCOPE type_name_struct_qualified
                                   | type_name_id_qualified
    """
    if len(p) > 2:
        p[0] = ((p[1],) + p[3][1], True, p[3][2])
    else:
        p[0] = p[1]


def p_object_name_namespace_root(p):
    """
        object_name : SCOPE object_name_qualified
        type_name : SCOPE type_name_qualified
    """
    p[0] = ((None,) + p[2][1], True, p[2][2])


def p_object_name(p):
    """
        object_name : METHOD_ID                                                         %prec PRIO0
                    | VARIABLE_ID                                                       %prec PRIO0

        object_name_id_qualified : METHOD_ID                                            %prec PRIO0
                                 | VARIABLE_ID                                          %prec PRIO0

        type_name : STRUCT_ID                                                           %prec PRIO0
                  | TYPENAME_ID                                                         %prec PRIO0
                  | STRUCT_ID_SHADOW                                                    %prec PRIO0
                  | TYPENAME_ID_SHADOW                                                  %prec PRIO0

        type_name_id_qualified : STRUCT_ID                                              %prec PRIO0
                               | TYPENAME_ID                                            %prec PRIO0
    """
    p[0] = ((p[1],), True, p.slice[1].found_object)


def p_object_name_shadow(p):
    """
        object_name : METHOD_ID_SHADOW                                                  %prec PRIO0
                    | VARIABLE_ID_SHADOW                                                %prec PRIO0
    """
    p[0] = ((p[1],), False, p.slice[1].found_object)


def p_object_name_id(p):
    """
        object_name : ID                                                                %prec PRIO0
    """
    p[0] = ((p[1],), False, None)

