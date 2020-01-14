precedence = (
    ('right', 'ASSIGN', 'ADD_ASSIGN', 'SUBSTRACT_ASSIGN', 'MULTIPLY_ASSIGN', 'DIVIDE_ASSIGN', 'MODULO_ASSIGN',
     'LEFT_SHIFT_ASSIGN', 'RIGHT_SHIFT_ASSIGN', 'BITWISE_AND_ASSIGN', 'BITWISE_OR_ASSIGN', 'BITWISE_XOR_ASSIGN',
     'TERNARY'),
    ('left', 'LOGICAL_OR'),
    ('left', 'LOGICAL_AND'),
    ('left', 'BITWISE_OR'),
    ('left', 'BITWISE_XOR'),
    ('left', 'BITWISE_AND'),
    ('left', 'EQUAL', 'NOT_EQUAL'),
    ('left', 'LESS_THAN', 'GREATER_THAN', 'LESS_THAN_OR_EQUAL', 'GREATER_THAN_OR_EQUAL'),
    ('left', 'LEFT_SHIFT', 'RIGHT_SHIFT'),
    ('left', 'ADD', 'SUBSTRACT'),
    ('left', 'MULTIPLY', 'DIVIDE', 'MODULO'),
    ('right', 'INCREMENT', 'DECREMENT', 'UADD', 'USUBSTRACT', 'CCAST', 'BITWISE_NOT', 'LOGICAL_NOT', 'ADDRESS_OF',
     'PTR'),
    ('left', 'PINCREMENT', 'PDECREMENT', 'PERIOD', 'DEREFERENCE', 'FUNCTION_CALL', 'ARRAY'),
    ('left', 'GROUP'),
    ('left', 'SCOPE2'),
    ('left', 'SCOPE'),
)


def p_unary_operator(p):
    """
        value : ADD value                                                   %prec UADD
              | SUBSTRACT value                                             %prec USUBSTRACT
              | MULTIPLY value                                              %prec PTR
              | LOGICAL_NOT value
              | BITWISE_NOT value
              | BITWISE_AND value                                           %prec ADDRESS_OF
              | INCREMENT value
              | DECREMENT value
              | value INCREMENT                                             %prec PINCREMENT
              | value DECREMENT                                             %prec PDECREMENT
    """
    p[0] = '%s %s' % (p[1], p[2])


def p_cast_operator(p):
    """
        value : LEFT_PARENTHESIS value RIGHT_PARENTHESIS value              %prec CCAST
    """
    p[0] = '%s%s%s%s' % (p[1], p[2], p[3], p[4])


def p_index_operator(p):
    """
        value : value LEFT_BRACKET value RIGHT_BRACKET                      %prec ARRAY
    """
    p[0] = '%s%s%s%s' % (p[1], p[2], p[3], p[4])


def p_call_operator(p):
    """
        value : value LEFT_PARENTHESIS skip_parameters RIGHT_PARENTHESIS    %prec FUNCTION_CALL
    """
    p[0] = '%s%s%s%s' % (p[1], p[2], p[3], p[4])


def p_binary_operator(p):
    """
        value : value ADD value
              | value SUBSTRACT value
              | value MULTIPLY value
              | value DIVIDE value
              | value MODULO value
              | value BITWISE_OR value
              | value BITWISE_AND value
              | value BITWISE_XOR value
              | value LEFT_SHIFT value
              | value RIGHT_SHIFT value
              | value LOGICAL_OR value
              | value LOGICAL_AND value
              | value LESS_THAN_OR_EQUAL value
              | value GREATER_THAN_OR_EQUAL value
              | value EQUAL value
              | value NOT_EQUAL value
              | value ASSIGN value
              | value ADD_ASSIGN value
              | value SUBSTRACT_ASSIGN value
              | value MULTIPLY_ASSIGN value
              | value DIVIDE_ASSIGN value
              | value MODULO_ASSIGN value
              | value LEFT_SHIFT_ASSIGN value
              | value RIGHT_SHIFT_ASSIGN value
              | value BITWISE_AND_ASSIGN value
              | value BITWISE_OR_ASSIGN value
              | value BITWISE_XOR_ASSIGN value
              | value DEREFERENCE value
              | value PERIOD value
    """
    p[0] = '%s %s %s' % (p[1], p[2], p[3])


def p_ternary_operator(p):
    """
        value : value CONDITIONAL value COLON value                         %prec TERNARY
    """
    p[0] = '%s%s%s%s%s' % (p[1], p[2].p[3], p[4], p[5])


def p_value_constant(p):
    """
        value_constant : CHARACTER
                       | WIDE_CHARACTER
                       | STRING
                       | WIDE_STRING
                       | FLOAT
                       | DECIMAL
                       | OCTAL
                       | HEX
    """
    p[0] = p[1]


def p_value(p):
    """
        value : value_constant
              | name
    """
    p[0] = p[1]


def p_value_parenthesis(p):
    """
        value : LEFT_PARENTHESIS value RIGHT_PARENTHESIS                    %prec GROUP
    """
    p[0] = '%s%s%s' % (p[1], p[2], p[3])
