from ..tree import Method, Operator, Constructor, Destructor, Parameter


def p_method_operator(p):
    """
        method_operator : ADD
                        | SUBSTRACT
                        | MULTIPLY
                        | DIVIDE
                        | MODULO
                        | BITWISE_OR
                        | BITWISE_AND
                        | BITWISE_NOT
                        | BITWISE_XOR
                        | LEFT_SHIFT
                        | RIGHT_SHIFT
                        | LOGICAL_OR
                        | LOGICAL_AND
                        | LOGICAL_NOT
                        | LESS_THAN
                        | GREATER_THAN
                        | LESS_THAN_OR_EQUAL
                        | GREATER_THAN_OR_EQUAL
                        | EQUAL
                        | NOT_EQUAL
                        | ASSIGN
                        | ADD_ASSIGN
                        | SUBSTRACT_ASSIGN
                        | MULTIPLY_ASSIGN
                        | DIVIDE_ASSIGN
                        | MODULO_ASSIGN
                        | LEFT_SHIFT_ASSIGN
                        | RIGHT_SHIFT_ASSIGN
                        | BITWISE_AND_ASSIGN
                        | BITWISE_OR_ASSIGN
                        | BITWISE_XOR_ASSIGN
                        | INCREMENT
                        | DECREMENT
                        | DEREFERENCE
                        | COMMA
    """
    p[0] = p[1]


def p_method_operator_2(p):
    """
        method_operator : LEFT_BRACKET RIGHT_BRACKET
                        | LEFT_PARENTHESIS RIGHT_PARENTHESIS
    """
    p[0] = '%s%s' % (p[1], p[2])


def p_method_return_type(p):
    """
        method_return_type : type
                           | VOID
    """
    p[0] = p[1]


def p_method_arg_optional_id(p):
    """
        method_arg_optional_id : ID
                               |
    """
    if len(p) > 1:
        p[0] = p[1]
    else:
        p[0] = ''


def p_method_arg_optional_value(p):
    """
    method_arg_optional_value : ASSIGN value
                              |
    """
    if len(p) > 1:
        p[0] = p[2]
    else:
        p[0] = None


def p_method_arg_array(p):
    """
        method_arg_array : LEFT_BRACKET value RIGHT_BRACKET
    """
    p[0] = '%s%s%s' % (p[1], p[2], p[3])


def p_method_arg_array_novalue(p):
    """
        method_arg_array : LEFT_BRACKET RIGHT_BRACKET
    """
    p[0] = '%s%s' % (p[1], p[2])


def p_method_arg_optional_array(p):
    """
    method_arg_optional_array : method_arg_array method_arg_optional_array
                              |
    """
    if len(p) > 1:
        p[0] = '%s%s' % (p[1], p[2])
    else:
        p[0] = ''


def p_method_arg(p):
    """
        method_arg : tag_list type method_arg_optional_id method_arg_optional_array method_arg_optional_value
    """
    p[0] = Parameter(p[2] + p[4], p[3], p[5])
    p[0].add_tags(p[1])


def p_method_arg_list(p):
    """
        method_arg_list : method_arg doc_left COMMA doc_left method_arg_list
                        | method_arg doc_left
    """
    if p[2]:
        p[1].add_tag(p[2])
    if len(p) > 3:
        if p[4]:
            p[1].add_tag(p[4])
        p[0] = [p[1]] + p[5]
    else:
        p[0] = [p[1]]


def p_method_arg_list_end(p):
    """
        method_arg_list : ELLIPSIS
                        |
    """
    if len(p) > 1:
        p[0] = [p[1]]
    else:
        p[0] = []


def p_method_args(p):
    """
        method_args : method_arg_list
                    | tag_list VOID
    """
    if len(p) == 3:
        p[0] = []
    else:
        p[0] = p[1]


def p_attribute_left(p):
    """
        attribute_left : VIRTUAL
                       | STATIC
                       | EXTERN
                       | INLINE
                       | EXPLICIT
                       | MUTABLE
                       | __HOST
                       | __DEVICE
                       | __KERNEL
    """
    p[0] = p[1]


def p_attribute_kw_list(p):
    """
        attribute_kw_list : attribute_left attribute_left_list
                          |
    """
    if len(p) > 1:
        p[0] = ([p[1]] + p[2][0], p[2][1])
    else:
        p[0] = ([], [])


def p_attribute_left_list(p):
    """
        attribute_left_list : tag_list template_decl_list attribute_kw_list
    """
    p[0] = (p[2] + p[3][0], p[1] + p[3][1])


def p_method_attribute_right(p):
    """
        method_attribute_right : CONST
                               | VOLATILE
                               | OVERRIDE
                               | ASSIGN DECIMAL
                               | THROW LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS
    """
    p[0] = p[1]


def p_method_attribute_right_list(p):
    """
        method_attribute_right_list : method_attribute_right method_attribute_right_list
                                    |
    """
    if len(p) > 1:
        p[0] = [p[1]] + p[2]
    else:
        p[0] = []


def p_constructor_arg_list(p):
    """
        constructor_arg_list : COMMA ID LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS constructor_arg_list
                             |
    """


def p_constructor_args(p):
    """
        constructor_args : COLON ID LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS constructor_arg_list
    """


def p_constructor_args_opt(p):
    """
        constructor_args_opt : constructor_args
                             |
    """


def p_method_prototype_operator(p):
    """
        method_prototype : method_return_type name_operator method_operator LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS method_attribute_right_list
    """
    if not p[2]:
        p[0] = Operator(p[3], p[1], p[5], p[7])


def p_method_prototype_operator_cast(p):
    """
        method_prototype : name_operator type LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS method_attribute_right_list
    """
    if not p[1]:
        p[0] = Method('#%s' % p[2], p[2], p[4], p[6])


def p_method_prototype_constructor(p):
    """
        method_prototype : method_return_type LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS method_attribute_right_list constructor_args_opt
    """
    if p[1].find('::') == -1:
        p[0] = Constructor(p.parser.stack[-1], p[1], p[1], p[3], p[5])


def p_method_prototype_destructor(p):
    """
        method_prototype : name_destructor ID LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS method_attribute_right_list
    """
    if not p[1]:
        p[0] = Destructor(p[2], 'void', p[4], p[6])


def p_method_prototype(p):
    """
        method_prototype : method_return_type name LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS method_attribute_right_list
    """
    if p[2].find('::') == -1:
        p[0] = Method(p[2], p[1], p[4], p[6])


def p_method_decl(p):
    """
        method_decl : attribute_left_list method_prototype
    """
    if p[2]:
        p[2].add_attributes(p[1][0])
        p[2].add_tags(p[1][1])
        if not 'template' in p[2].attributes:
            p.parser.stack[-1].add_method(p[2])


def p_method_pointer_name_opt(p):
    """
        method_pointer_name_opt : ID
                                |
    """


def p_method_pointer(p):
    """
        method_pointer : method_return_type LEFT_PARENTHESIS MULTIPLY method_pointer_name_opt RIGHT_PARENTHESIS LEFT_PARENTHESIS method_args RIGHT_PARENTHESIS
    """


def p_expr_method(p):
    """
        expr : method_decl SEMICOLON
             | method_decl LEFT_BRACE skip_body RIGHT_BRACE
    """
