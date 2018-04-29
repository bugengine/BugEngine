# Type lookup
def p_type_name(p):
    """
        type_name : STRUCT_ID                                                           %prec PRIO0
                  | TYPENAME_ID                                                         %prec PRIO0
                  | STRUCT_ID_SHADOW                                                    %prec PRIO0
                  | TYPENAME_ID_SHADOW                                                  %prec PRIO0

        type_name_id_qualified : STRUCT_ID                                              %prec PRIO0
                               | TYPENAME_ID                                            %prec PRIO0
    """
    p[0] = (p[1], p.slice[1].found_object)


def p_type_name_namespace(p):
    """
        type_name : NAMESPACE_ID SCOPE type_name_qualified
                  | NAMESPACE_ID_SHADOW SCOPE type_name_qualified
                  | STRUCT_ID SCOPE type_name_struct_qualified
                  | STRUCT_ID_SHADOW SCOPE type_name_struct_qualified
    """
    p[0] = p[3]


def p_type_name_namespace_root(p):
    """
        type_name : SCOPE type_name_qualified
    """
    p[0] = p[2]



def p_type_name_qualified(p):
    """
        type_name_qualified : NAMESPACE_ID SCOPE type_name_qualified
                            | type_name_struct_qualified

        type_name_struct_qualified : STRUCT_ID SCOPE type_name_struct_qualified
                                   | type_name_id_qualified
    """
    if len(p) > 2:
        p[0] = p[3]
    else:
        p[0] = p[1]


# Object lookup
def p_object_name_namespace(p):
    """
        object_name : NAMESPACE_ID SCOPE object_name_qualified
                    | NAMESPACE_ID_SHADOW SCOPE object_name_qualified
                    | STRUCT_ID SCOPE object_name_struct_qualified
                    | STRUCT_ID_SHADOW SCOPE object_name_struct_qualified
    """
    p[0] = ((p[1],) + p[3][1], True, p[3][2])


def p_object_name_struct_qualified(p):
    """
        object_name_qualified : NAMESPACE_ID SCOPE object_name_qualified
                              | object_name_struct_qualified
        object_name_struct_qualified : STRUCT_ID SCOPE object_name_struct_qualified
                                     | object_name_id_qualified
    """
    if len(p) > 2:
        p[0] = ((p[1],) + p[3][1], True, p[3][2])
    else:
        p[0] = p[1]


def p_object_name_namespace_root(p):
    """
        object_name : SCOPE object_name_qualified
    """
    p[0] = ((None,) + p[2][1], True, p[2][2])


def p_object_name(p):
    """
        object_name : NAMESPACE_ID
                    | STRUCT_ID
                    | TYPENAME_ID
                    | METHOD_ID
                    | VARIABLE_ID
                    | TEMPLATE_ID

        object_name_id_qualified : STRUCT_ID
                                 | TYPENAME_ID
                                 | METHOD_ID
                                 | VARIABLE_ID
                                 | TEMPLATE_ID
    """
    p[0] = (p[1], True, p.slice[1].found_object)


def p_object_name_shadow(p):
    """
        object_name : NAMESPACE_ID_SHADOW
                    | STRUCT_ID_SHADOW
                    | TYPENAME_ID_SHADOW
                    | METHOD_ID_SHADOW
                    | VARIABLE_ID_SHADOW
                    | TEMPLATE_ID_SHADOW
    """
    p[0] = (p[1], True, p.slice[1].found_object)


def p_object_name_id(p):
    """
        object_name : ID
    """
    p[0] = (p[1], False, None)

