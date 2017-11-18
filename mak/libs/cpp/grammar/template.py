def p_template_decl(p):
    """
        template_decl : TEMPLATE LESS_THAN skip_template_args GREATER_THAN
    """

def p_template_decl_list(p):
    """
        template_decl_list : template_decl template_decl_list
    """
    p[0] = ['template']

def p_template_decl_list_empty(p):
    """
        template_decl_list :
    """
    p[0] = []
