def p_friend(p):
    """
        friend : FRIEND method_prototype
               | FRIEND type_declaration
    """


def p_friend_expr(p):
    """
        expr : attribute_left_list friend SEMICOLON
    """
