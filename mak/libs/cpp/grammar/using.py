def p_using(p):
    """
        using : USING NAMESPACE name SEMICOLON
              | USING name SEMICOLON
    """
    if len(p) == 5:
        p.lexer.usings.append(p[3])


def p_using_expr(p):
    """
        expr : using
    """
