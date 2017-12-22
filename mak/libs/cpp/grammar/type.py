from ..tree import Typedef

def p_type_kw(p):
    """
        type_kw : INT
                | SHORT
                | CHAR
                | LONG
                | SIGNED
                | UNSIGNED
    """
    p[0] = p[1]


def p_type_builtin_list(p):
    """
        type_builtin_list : type_kw type_builtin_list
                          |
    """
    if len(p) > 1:
        p[0] = '%s %s'%(p[1], p[2])
    else:
        p[0] = ''


def p_type_attribute(p):
    """
        type_attribute : CONST
                       | VOLATILE
    """
    p[0] = p[1]


def p_type_attribute_list(p):
    """
        type_attribute_list : type_attribute type_attribute_list
                            |
    """
    if len(p) > 1:
        p[0] = '%s %s' %(p[1], p[2])
    else:
        p[0] = ''


def p_type_access(p):
    """
        type_access : MULTIPLY
                    | BITWISE_AND
    """
    p[0] = p[1]


def p_type_access_list(p):
    """
        type_access_list : type_attribute_list type_access type_access_list
                         | type_attribute_list
    """
    if len(p) == 4:
        p[0] = '%s%s%s' % (p[1], p[2], p[3])
    else:
        p[0] = p[1]


def p_type(p):
    """
        type : name type_access_list
             | type_declaration type_access_list
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_type_from_definition(p):
    """
        type : type_definition type_access_list
    """
    p[0] = '%s %s' % (p[1].cpp_name(), p[2])


def p_type_keywords(p):
    """
        type : type_kw type_builtin_list type_access_list
    """
    p[0] = '%s %s%s' % (p[1], p[2], p[3])


def p_type_void(p):
    """
        type : VOID type_access type_access_list
    """
    p[0] = '%s %s%s' % (p[1], p[2], p[3])


def p_type_with_attribute(p):
    """
        type : type_attribute type
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_type_expr_definition(p):
    """
        expr : attribute_left_list type_definition SEMICOLON
    """
    p[2].add_tags(p[1][1])


def p_type_expr_declaration(p):
    """
        expr : attribute_left_list type_declaration SEMICOLON
    """


def p_typedef_expr(p):
    """
        expr : attribute_left_list TYPEDEF type ID variable_array_list_opt doc_left SEMICOLON
             | attribute_left_list TYPEDEF method_pointer doc_left SEMICOLON
    """
    if len(p) == 8:
        p[0] = Typedef(p[4], p[3]+p[5])
        p[0].add_tags(p[1][1])
        p.parser.stack[-1].add_property(p[0])
