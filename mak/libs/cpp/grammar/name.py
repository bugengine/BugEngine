def p_name_item_id(p):
    """
        name_item : ID
    """
    p[0] = p[1]


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
                  | name_item_extended                                          %prec SCOPE2
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
        p[0] = '%s%s' % (p[1], p[2])


def p_name_typename(p):
    """
        name : TYPENAME name_item SCOPE name_list
             | TYPENAME SCOPE name_list
    """

    if len(p) > 4:
        p[0] = '%s %s%s%s' % (p[1], p[2], p[3], p[4])
    else:
        p[0] = '%s %s%s' % (p[1], p[2], p[3])


def p_name_single_item(p):
    """
        name : name_item                                                        %prec SCOPE2
    """
    p[0] = p[1]


def p_name_list_opt(p):
    """
        name_list_opt : name_item_extended SCOPE name_list_opt
                      |
    """


def p_name_operator(p):
    """
        name_operator : name_item SCOPE name_list_opt OPERATOR
                      | SCOPE OPERATOR
                      | OPERATOR
    """


def p_name_destructor(p):
    """
        name_destructor : name_item SCOPE name_list_opt BITWISE_NOT
                        | SCOPE BITWISE_NOT
                        | BITWISE_NOT
    """
