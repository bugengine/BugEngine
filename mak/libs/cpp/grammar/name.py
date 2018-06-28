def p_name_item_id(p):
    """
        name_item : ID
    """
    p[0] = p[1]


def p_name_item_typename(p):
    """
        name_item : TYPENAME ID
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_name_item_template(p):
    """
        name_item : ID LESS_THAN skip_template_args GREATER_THAN
    """
    p[0] = '%s%s %s %s' % (p[1], p[2], p[3], p[4])


def p_name_item_extended(p):
    """
        name_item_extended : TEMPLATE ID LESS_THAN skip_template_args GREATER_THAN
    """
    p[0] = '%s %s%s %s %s' % (p[1], p[2], p[3], p[4], p[5])


def p_name_item_extended_2(p):
    """
        name_item_extended : name_item
    """
    p[0] = p[1]


def p_name_list(p):
    """
        name_list : name_item_extended SCOPE name_list
                  | name_item_extended                                          %prec GROUP
    """
    if len(p) > 3:
        p[0] = '%s%s%s' % (p[1], p[2], p[3])
    else:
        p[0] = p[1]


def p_name(p):
    """
        name : name_item SCOPE name_list
             | SCOPE name_list
    """
    if len(p) > 3:
        p[0] = '%s%s%s' % (p[1], p[2], p[3])
    else:
        p[0] = '%s%s%s' % (p[1], p[2])


def p_name_single_item(p):
    """
        name : name_item                                                        %prec GROUP
    """
    p[0] = p[1]


def p_name_operator(p):
    """
        name_operator : name_item SCOPE name_list SCOPE OPERATOR
                      | name_item SCOPE OPERATOR
                      | SCOPE OPERATOR
                      | OPERATOR
    """
    #print(p.slice[0])


def p_name_destructor(p):
    """
        name_destructor : name_item SCOPE name_list SCOPE BITWISE_NOT
                        | name_item SCOPE BITWISE_NOT
                        | SCOPE BITWISE_NOT
                        | BITWISE_NOT
    """
