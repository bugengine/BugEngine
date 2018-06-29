def p_expr_ignore(p):
    """
        expr : SEMICOLON
    """


def p_expr_error(p):
    """
        expr : error SEMICOLON
    """


def p_exprs_end(p):
    """
        exprs :
    """
    p[0] = {
        'methods': '{0}',
        'objects': '{0}',
        'properties': '{0}'
    }


def p_exprs(p):
    """
        exprs : expr exprs
    """
    p[0] = p[1]
