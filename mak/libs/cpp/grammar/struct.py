from ..tree import Class, AnonymousClass


def p_struct_visibility(p):
    """
        struct_visibility : PUBLISHED
                          | PUBLIC
                          | PROTECTED
                          | PRIVATE
    """
    p[0] = p[1]


def p_struct_visibility_opt(p):
    """
        struct_visibility_opt : struct_visibility
                              |
    """
    if len(p) > 1:
        p[0] = p[1]
    else:
        p[0] = ''


def p_parent_list_item(p):
    """
        struct_parent_list : struct_visibility_opt name
                           | struct_visibility_opt name COMMA struct_parent_list
    """
    p[0] = [(p[1], p[2])]
    if len(p) == 5:
        p[0] += p[4]


def p_struct_parent_decl(p):
    """
        struct_parent_decl : COLON struct_parent_list
                           |
    """
    if len(p) > 1:
        p[0] = p[2]
    else:
        p[0] = []


def p_struct_optional_id(p):
    """
        struct_optional_id : ID
                           |
    """
    if len(p) == 2:
        p[0] = p[1]


def p_struct_type(p):
    """
        struct_type : STRUCT
                    | CLASS
                    | UNION
                    | BE_POD
    """
    p[0] = p[1]


def p_struct_header(p):
    """
        struct_header : struct_type struct_optional_id struct_parent_decl
    """
    if p[2] and not p.parser.stack[-1].anonymous:
        p[0] = Class(p.parser.stack[-1].name + [p[2]], p[1], p[3])
    else:
        p[0] = AnonymousClass()
    p.parser.stack.append(p[0])


def p_struct_visibility_header(p):
    """
        struct_visibility_header : struct_visibility COLON
    """
    p.parser.stack[-1].set_visibility(p[1])


def p_struct_visibility_block(p):
    """
        struct_visibility_block :   struct_visibility_header exprs struct_visibility_block
                                |
    """


def p_struct_members(p):
    """
        struct_members : exprs struct_visibility_block
    """


def p_struct_def(p):
    """
        struct_definition : struct_header LEFT_BRACE struct_members RIGHT_BRACE
    """
    p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])
    p[0] = p[1]


def p_struct_def_error(p):
    """
        struct_definition : struct_header LEFT_BRACE error RIGHT_BRACE
    """
    p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])
    p[0] = p[1]


def p_struct_type_declaration(p):
    """
        type_declaration :   struct_type ID
    """
    p[0] = p[2]


def p_struct_type_definition(p):
    """
        type_definition :   struct_definition
    """
    p[0] = p[1]
