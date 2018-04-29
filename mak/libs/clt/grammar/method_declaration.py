
def p_parameter_name_none(p):
    """
        parameter_name :
    """
    p[0] = None


def p_parameter_name(p):
    """
        parameter_name : ID
                       | STRUCT_ID_SHADOW
                       | TYPENAME_ID_SHADOW
                       | NAMESPACE_ID_SHADOW
                       | METHOD_ID_SHADOW
                       | VARIABLE_ID_SHADOW
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_parameter_name_invalid(p):
    """
        parameter_name : STRUCT_ID
                       | TYPENAME_ID
                       | NAMESPACE_ID
                       | METHOD_ID
                       | VARIABLE_ID
    """
    p.lexer._error("redefinition of '%s' as different kind of symbol" % (p[1]), p.position(1))
    p.lexer._note("previous definition is here", p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


def p_method_parameter_type(p):
    """
        method_parameter_type : type
    """
    p[0] = p[1]


def p_method_parameter_list_last(p):
    """
        method_parameter_list : method_parameter_type parameter_name
    """


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name COMMA method_parameter_list
    """


def p_method_parameters(p):
    """
        method_parameters : method_parameter_list
                          | VOID
                          |
    """


def p_push_method_scope(p):
    """
        push_method_scope :
    """


def p_pop_method_scope(p):
    """
        pop_method_scope :
    """


def p_method_attribute(p):
    """
        method_attribute : CONST
    """


def p_method_attributes(p):
    """
        method_attributes :
                          | method_attribute method_attributes
    """


def p_method_declaration(p):
    """
        method_declaration : declaration_specifier_list type object_name LPAREN push_method_scope method_parameters RPAREN method_attributes pop_method_scope
                           | declaration_specifier_list typedecl object_name LPAREN push_method_scope method_parameters RPAREN method_attributes pop_method_scope
                           | declaration_specifier_list VOID object_name LPAREN push_method_scope method_parameters RPAREN method_attributes pop_method_scope
    """


def p_method_definition(p):
    """
        method_definition : method_declaration statement_block
    """

