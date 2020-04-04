def p_skip_keyword(p):
    """
        skip_keyword : CLASS
                     | STRUCT
                     | ENUM
                     | BE_POD
                     | UNION
                     | NAMESPACE
                     | USING
                     | NEW
                     | DELETE
                     | PUBLISHED
                     | PUBLIC
                     | PROTECTED
                     | PRIVATE
                     | FRIEND
                     | UNSIGNED
                     | SIGNED
                     | CHAR
                     | SHORT
                     | INT
                     | LONG
                     | VOID
                     | EXPLICIT
                     | INLINE
                     | EXTERN
                     | STATIC
                     | MUTABLE
                     | CONST
                     | VOLATILE
                     | VIRTUAL
                     | OVERRIDE
                     | TEMPLATE
                     | TYPENAME
                     | OPERATOR
                     | TYPEDEF
                     | THROW
                     | __GLOBAL
                     | __LOCAL
                     | __CONSTANT
                     | __DEVICE
                     | BE_TAG
                     | BE_SEGMENT
                     | BE_SEGMENTS
                     | BE_STREAM
    """
    p[0] = p[1]


def p_skip_operator(p):
    """
        skip_operator : ADD
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
                      | LESS_THAN_OR_EQUAL
                      | GREATER_THAN_OR_EQUAL
                      | EQUAL
                      | NOT_EQUAL
                      | SCOPE
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
                      | CONDITIONAL
                      | LEFT_BRACKET
                      | RIGHT_BRACKET
                      | COMMA
                      | PERIOD
                      | SEMICOLON
                      | COLON
                      | ELLIPSIS
    """
    p[0] = p[1]


def p_skip_constant(p):
    """
        skip_constant : value_constant
    """
    p[0] = p[1]


def p_skip_base(p):
    """
        skip_base : skip_operator
                  | skip_keyword
                  | skip_constant
                  | ID
    """
    p[0] = p[1]


def p_skip_extra(p):
    """
        skip_extra : skip_operator
                   | skip_keyword
                   | skip_constant
                   | ID
                   | LESS_THAN
                   | GREATER_THAN
    """
    p[0] = p[1]


def p_skip_doxy(p):
    """
        skip_doxy : DOXYGEN_NEWLINE skip_doxy
                  | DOXYGEN_WORD skip_doxy
                  | DOXYGEN_LIST skip_doxy
                  |
    """
    p[0] = ''


def p_skip_template_args_empty(p):
    """
        skip_template_args :
    """
    p[0] = ''


def p_skip_template_args(p):
    """
        skip_template_args : skip_base skip_template_args
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_skip_template_args_extra(p):
    """
        skip_template_args : LESS_THAN skip_template_args GREATER_THAN skip_template_args
                           | LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS skip_template_args
                           | DOXYGEN_BEGIN skip_doxy DOXYGEN_END skip_template_args
                           | DOXYGEN_BEGIN_LEFT skip_doxy DOXYGEN_END skip_template_args
    """
    p[0] = '%s %s %s %s' % (p[1], p[2], p[3], p[4])


def p_skip_parameters_empty(p):
    """
        skip_parameters :
    """
    p[0] = ''


def p_skip_parameters(p):
    """
        skip_parameters : skip_extra skip_parameters
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_skip_parameters_extra(p):
    """
        skip_parameters : LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS skip_parameters
    """
    p[0] = '%s%s%s%s' % (p[1], p[2], p[3], p[4])


def p_skip_parameters_ignore(p):
    """
        skip_parameters : DOXYGEN_BEGIN skip_doxy DOXYGEN_END skip_parameters
                        | DOXYGEN_BEGIN_LEFT skip_doxy DOXYGEN_END skip_parameters
    """
    p[0] = p[4]


def p_skip_body(p):
    """
        skip_body : skip_extra skip_body
                  | LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS skip_body
                  | LEFT_BRACE skip_body RIGHT_BRACE skip_body
                  | DOXYGEN_BEGIN skip_doxy DOXYGEN_END skip_body
                  | DOXYGEN_BEGIN_LEFT skip_doxy DOXYGEN_END skip_body
                  |
    """
