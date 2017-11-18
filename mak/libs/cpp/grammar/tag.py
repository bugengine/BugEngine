def p_tag(p):
    """
        tag : BE_TAG LEFT_PARENTHESIS name LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS RIGHT_PARENTHESIS
    """
    p[0] = (p[3], p[5])


def p_tag_list(p):
    """
        tag_list : tag tag_list
    """
    if p[1]:
        p[0] = [p[1]] + p[2]
    else:
        p[0] = p[2]


def p_tag_list_empty(p):
    """
        tag_list :
    """
    p[0] = []
