from .. import cl_ast


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
                       | TEMPLATE_STRUCT_ID_SHADOW
                       | TEMPLATE_METHOD_ID_SHADOW
                       | TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_operator_name(p):
    """
        operator_name : OPERATOR PLUS
                      | OPERATOR MINUS
                      | OPERATOR TIMES
                      | OPERATOR DIVIDE
                      | OPERATOR MOD
                      | OPERATOR OR
                      | OPERATOR AND
                      | OPERATOR NOT
                      | OPERATOR XOR
                      | OPERATOR LSHIFT
                      | OPERATOR RSHIFT
                      | OPERATOR LOR
                      | OPERATOR LAND
                      | OPERATOR LNOT
                      | OPERATOR LT
                      | OPERATOR LE
                      | OPERATOR GT
                      | OPERATOR GE
                      | OPERATOR EQ
                      | OPERATOR NE
                      | OPERATOR EQUALS
                      | OPERATOR TIMESEQUAL
                      | OPERATOR DIVEQUAL
                      | OPERATOR MODEQUAL
                      | OPERATOR PLUSEQUAL
                      | OPERATOR MINUSEQUAL
                      | OPERATOR LSHIFTEQUAL
                      | OPERATOR RSHIFTEQUAL
                      | OPERATOR ANDEQUAL
                      | OPERATOR XOREQUAL
                      | OPERATOR OREQUAL
                      | OPERATOR PLUSPLUS
                      | OPERATOR MINUSMINUS
                      | OPERATOR ARROW
                      | OPERATOR LPAREN RPAREN
                      | OPERATOR LBRACKET RBRACKET
                      | OPERATOR PERIOD
                      | OPERATOR COMMA
    """
    p[0] = p[2]


def p_parameter_name_invalid(p):
    """
        parameter_name : STRUCT_ID
                       | TYPENAME_ID
                       | NAMESPACE_ID
                       | METHOD_ID
                       | VARIABLE_ID
                       | TEMPLATE_STRUCT_ID
                       | TEMPLATE_METHOD_ID
                       | TEMPLATE_TYPENAME_ID
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
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt
    """
    p.lexer.scopes[-1].add(cl_ast.methods.Parameter(p[1], p[2], p[3], [], p.position(2)))


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt COMMA method_parameter_list
    """
    p.lexer.scopes[-1].add(cl_ast.methods.Parameter(p[1], p[2], p[3], [], p.position(2)))


def p_method_parameters(p):
    """
        method_parameters : method_parameter_list
                          | VOID
                          |
    """


def p_create_method_scope(p):
    """
        create_method_scope :
    """
    p[0] = cl_ast.methods.Method(p[-3], p[-4], p[-5], p.position(-1))


def p_create_constructor_scope(p):
    """
        create_constructor_scope :
    """
    p[0] = cl_ast.methods.Method(p[-3], p[-3], [p[-4]], p.position(-1))


def p_create_castop_scope(p):
    """
        create_castop_scope :
    """
    p[0] = cl_ast.methods.Method(p[-1], p[-1], [p[-3]], p.position(-1))


def p_create_method_definition(p):
    """
        create_method_definition :
    """
    p[0] = cl_ast.methods.Body(p.position(-1))
    p.lexer.scopes[-1].define(p[0])


def p_push_method_scope(p):
    """
        push_method_scope :
    """
    p.lexer.push_scope(p[-1])
    p[0] = p[-1]


def p_pop_method_scope(p):
    """
        pop_method_scope :
    """
    p.lexer.pop_scope()


def p_method_attribute(p):
    """
        method_attribute : CONST
    """


def p_method_attributes(p):
    """
        method_attributes :
                          | method_attribute method_attributes
    """


def p_initializer(p):
    """
        initializer : object_name LPAREN expression_list RPAREN
    """


def p_initializer_list(p):
    """
        initializer_list : initializer
                         | initializer COMMA initializer_list
    """


def p_initializer_list_opt(p):
    """
        initializer_list_opt : COLON initializer_list
                             |
    """


def p_method_declaration_prefix(p):
    """
        method_declaration_prefix : declaration_specifier_list type object_name create_method_scope
                                  | declaration_specifier_list VOID object_name create_method_scope
                                  | declaration_specifier_list type operator_name create_method_scope
                                  | declaration_specifier_list VOID operator_name create_method_scope
                                  | declaration_specifier_list OPERATOR type create_castop_scope
    """
    p[0] = p[4]


def p_method_declaration_prefix_ctor(p):
    """
        method_declaration_prefix : declaration_specifier_list STRUCT_ID_SHADOW create_constructor_scope
                                  | declaration_specifier_list TEMPLATE_STRUCT_ID_SHADOW create_constructor_scope
    """
    p[0] = p[3]


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix push_method_scope LPAREN method_parameters RPAREN method_attributes pop_method_scope
    """
    p[0] = p[2]


def p_method_definition(p):
    """
        method_definition : method_declaration push_method_scope create_method_definition initializer_list_opt statement_block pop_method_scope
    """
    p[0] = p[1]

