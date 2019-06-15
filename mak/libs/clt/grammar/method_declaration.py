

def p_parameter_name_none(p):
    """
        parameter_name :
    """


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


def p_method_parameter_type(p):
    """
        method_parameter_type : type
    """


def p_method_parameter_list_last(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt
    """


def p_method_parameter_list(p):
    """
        method_parameter_list : method_parameter_type parameter_name variable_initial_value_opt COMMA method_parameter_list
    """


def p_method_parameters(p):
    """
        method_parameters : method_parameter_list
    """


def p_method_parameters_none(p):
    """
        method_parameters : VOID
                          |
    """


def p_create_method(p):
    """
        create_method :
    """


def p_create_constructor(p):
    """
        create_constructor :
    """


def p_create_castop(p):
    """
        create_castop :
    """


def p_create_method_definition(p):
    """
        create_method_definition :
    """


def p_push_method_scope(p):
    """
        push_method_scope :
    """


def p_push_overload_scope(p):
    """
        push_overload_scope :
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


def p_initializer(p):
    """
        initializer : object_name LPAREN expression_list_opt RPAREN
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
        method_declaration_prefix : declaration_specifier_list type consume_template_stack object_name verify_template_stack create_method
    """


def p_method_declaration_prefix_void(p):
    """
        method_declaration_prefix : declaration_specifier_list VOID consume_template_stack object_name verify_template_stack create_method
    """


def p_method_declaration_prefix_cast_operator(p):
    """
        method_declaration_prefix : declaration_specifier_list cast_method_name create_castop
    """


def p_method_declaration_prefix_ctor(p):
    """
        method_declaration_prefix : declaration_specifier_list special_method_name
    """


def p_method_declaration_prefix_ctor_2(p):
    """
        method_declaration_prefix : declaration_specifier_list STRUCT_ID_SHADOW create_constructor
                                  | declaration_specifier_list TEMPLATE_STRUCT_ID_SHADOW create_constructor
    """


def p_method_declaration(p):
    """
        method_declaration : method_declaration_prefix push_method_scope LPAREN method_parameters RPAREN method_attributes pop_method_scope
    """


def p_method_definition(p):
    """
        method_definition : method_declaration push_overload_scope create_method_definition initializer_list_opt statement_block pop_method_scope
    """

