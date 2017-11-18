from mak.libs.cpp.tree import Variable

def p_variable_array(p):
    """
        variable_array : LEFT_BRACKET value RIGHT_BRACKET
    """
    p[0] = '%s%s%s' % (p[1], p[2], p[3])


def p_variable_array_list_opt(p):
    """
        variable_array_list_opt : variable_array variable_array_list_opt
                                |
    """
    if len(p) > 1:
        p[0] = '%s%s' % (p[1], p[2])
    else:
        p[0] = ''


def p_variable_decl(p):
    """
        variable_decl : method_return_type ID variable_array_list_opt
    """
    p[0] = Variable(p[2], p[1]+p[3])


def p_variable_decl_method(p):
    """
        variable_decl : method_return_type LEFT_PARENTHESIS MULTIPLY ID RIGHT_PARENTHESIS LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS
    """


def p_variable_expr(p):
    """
        expr : attribute_left_list variable_decl SEMICOLON
    """
    if p[2]:
        p[2].add_attributes(p[1][0])
        p[2].add_tags(p[1][1])
    p.parser.stack[-1].add_property(p[2])
