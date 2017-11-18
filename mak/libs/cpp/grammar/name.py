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
        name_item_extended : name_item
    """
    p[0] = p[1]


def p_name_item_extended_template(p):
    """
        name_item_extended : TEMPLATE ID LESS_THAN skip_template_args GREATER_THAN
    """
    p[0] = '%s %s%s %s %s' % (p[1], p[2], p[3], p[4], p[5])


def p_name_list_extended(p):
    """
        name_list_extended : name_item_extended
                           | name_item_extended SCOPE name_list_extended
    """
    if len(p) > 2:
        p[0] = '%s%s%s' % (p[1], p[2], p[3])
    else:
        p[0] = p[1]


def p_name_list(p):
    """
        name_list : name_item
                  | name_item SCOPE name_list_extended
    """
    if len(p) > 2:
        p[0] = '%s%s%s' % (p[1], p[2], p[3])
    else:
        p[0] = p[1]


def p_name(p):
    """
        name : name_list
             | SCOPE name_list
    """
    if len(p) > 2:
        p[0] = '%s%s' % (p[1], p[2])
    else:
        p[0] = p[1]
