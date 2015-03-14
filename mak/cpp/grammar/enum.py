from cpp.tree import Class, AnonymousClass, EnumValue

def p_enum_value(p):
    """
        enum_value : ID
                   | ID ASSIGN value
    """
    p[0] = p[1]


def p_enum_values(p):
    """
        enum_values : tag_list enum_value doc_left COMMA doc_left enum_values
                    | tag_list enum_value doc_left
    """
    v = EnumValue(p[2], p.parser.stack[-1].cpp_name())
    v.add_tags(p[1])
    v.add_attributes(['static'])
    p.parser.stack[-1].add_property(v)


def p_enum_values_empty(p):
    """
        enum_values :
    """
    p[0] = []



def p_enum_name_opt(p):
    """
        enum_name_opt : ID
                      |
    """
    if len(p) > 1:
        p[0] = p[1]


def p_enum_header(p):
    """
        enum_header : ENUM enum_name_opt
    """
    if p[2] and not p.parser.stack[-1].anonymous:
        p[0] = Class(p.parser.stack[-1].name + [p[2]], p[1], [])
    else:
        p[0] = AnonymousClass()
    p.parser.stack.append(p[0])


def p_enum_definition(p):
    """
        enum_definition : enum_header LEFT_BRACE enum_values RIGHT_BRACE
    """
    p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])
    p[0] = p[1]


def p_enum_error(p):
    """
        enum_definition : enum_header LEFT_BRACE error RIGHT_BRACE
    """
    p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])
    p[0] = p[1]


def p_enum_type_declaration(p):
    """
        type_declaration :   ENUM ID
    """
    p[0] = p[2]


def p_enum_type_definition(p):
    """
        type_definition : enum_definition
    """
    p[0] = p[1]
