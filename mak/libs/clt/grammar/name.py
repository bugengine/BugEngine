
def p_root_namespace_begin(p):
    """
        root_namespace_begin : SCOPE
    """
    p[0] = p.lexer.scopes[0]

def p_qualfied_name_lookup(p):
    """
        qualified_type_name_lookup : root_namespace_begin unqualified_type_name_lookup
    """
    p[0] = p[1]

def p_namespace_begin(p):
    """
        namespace_begin : NAMESPACE_ID SCOPE
                        | NAMESPACE_ID_SHADOW SCOPE
    """
    p[0] = p.slice[1].found_object

def p_namespace_lookup_opt(p):
    """
        namespace_lookup_opt : namespace_begin namespace_lookup_opt
                             |
    """
    pass

def p_type_name(p):
    """
        type_name : STRUCT_ID
                  | STRUCT_ID_SHADOW
                  | TYPENAME_ID
                  | TYPENAME_ID_SHADOW
    """
    p[0] = p.slice[1].found_object

def p_type_name_list_last(p):
    """
        type_name_list : type_name
    """
    p[0] = p[1]

def p_type_name_list(p):
    """
        type_name_list : type_name SCOPE type_name_list
    """
    p[0] = p[1]

def p_unqualfied_name_lookup_typename(p):
    """
        unqualified_type_name_lookup : namespace_lookup_opt type_name_list
    """
    p[0] = p[2]

def p_decl_type(p):
    """
        type : unqualified_type_name_lookup
             | qualified_type_name_lookup
    """
    p[0] = p[1]

