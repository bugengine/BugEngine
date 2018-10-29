from .. import cl_ast


def p_value_float(p):
    """
        expression : FLOAT_CONST
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'float')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    if p[1][-1] == 'f':
        p[1] = p[1][:-1]
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, float(p[1]))


def p_value_float_hex(p):
    """
        expression : HEX_FLOAT_CONST
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'float')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', position)
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, float.fromhex(p[1]))


def int_to_value(parser, position, string_value, base):
    type = 'i32'
    while string_value[-1] in ('l', 'u'):
        if string_value[-1] == 'l':
            type = type[0] + '64'
        elif string_value[-1] == 'u':
            type = 'u' + type[1:]
        string_value = string_value[:-1]
    value = int(string_value, base)
    type = cl_ast.types.Builtin(parser, position, type)
    type = cl_ast.types.Type(parser, position, type)
    type.add_modifier('const', position)
    return cl_ast.values.Value(parser, position, type, value)


def p_value_int_10(p):
    """
        expression : INT_CONST_DEC
    """
    p[0] = int_to_value(p.lexer.scopes[-1], p.position(1), p[1], 10)


def p_value_int_8(p):
    """
        expression : INT_CONST_OCT
    """
    p[0] = int_to_value(p.lexer.scopes[-1], p.position(1), p[1], 8)


def p_value_int_16(p):
    """
        expression : INT_CONST_HEX
    """
    p[0] = int_to_value(p.lexer.scopes[-1], p.position(1), p[1][2:], 16)


def p_value_int_2(p):
    """
        expression : INT_CONST_BIN
    """
    p[0] = int_to_value(p.lexer.scopes[-1], p.position(1), p[1][2:], 2)


def p_value_char(p):
    """
        expression : CHAR_CONST
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'i8')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    str = eval(p[1])
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, str)


def p_value_wchar(p):
    """
        expression : WCHAR_CONST
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'i32')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    str = eval(p[1][1:])
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, str)


def p_value_string_literal(p):
    """
        expression : STRING_LITERAL
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'i8')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    type = cl_ast.types.Pointer(p.lexer.scopes[-1], p.position(1), type)
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    str = eval(p[1])
    str = str.encode('utf8')
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, str)


def p_value_wstring_literal(p):
    """
        expression : WSTRING_LITERAL
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'i32')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    type = cl_ast.types.Pointer(p.lexer.scopes[-1], p.position(1), type)
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    str = eval(p[1][1:])
    str = str.encode('utf32')
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, str)


def p_value_true(p):
    """
        expression : TRUE
                   | FALSE
    """
    type = cl_ast.types.Builtin(p.lexer.scopes[-1], p.position(1), 'bool')
    type = cl_ast.types.Type(p.lexer.scopes[-1], p.position(1), type)
    type.add_modifier('const', p.position(1))
    p[0] = cl_ast.values.Value(p.lexer.scopes[-1], p.position(1), type, p[1] == 'true')


def p_value_this(p):
    """
        expression : THIS
    """
    p.lexer.lookup(p.slice[1])
    try:
        p[0] = p.slice[1].found_object
    except AttributeError:
        p.lexer._error('invalid use of \'this\' outside of a non-static member function', p.position(1))
        p[0] = None


def p_value_object(p):
    """
        expression : object_name
    """
    if p[1].dependent:
        p[0] = cl_ast.values.DependentValueName(p.lexer.scopes[-1], p[1].position, p[1])
    else:
        p[0] = p[1].target
        if not p[0]:
            p.lexer._error('Unknown object: %s' % ('::'.join(p[1].name)), p[1].position)


def p_operator_expr(p):
    """
        expression : LPAREN expression_list RPAREN
                   | expression PLUSPLUS                                    %prec PRIO1
                   | expression MINUSMINUS                                  %prec PRIO1
                   | expression LPAREN expression_list_opt RPAREN           %prec PRIO1
                   | expression LBRACKET expression_list RBRACKET           %prec PRIO1
                   | expression PERIOD object_name                          %prec PRIO1
                   | expression ARROW object_name                           %prec PRIO1
                   | PLUSPLUS expression                                    %prec PRE_PLUSPLUS
                   | MINUSMINUS expression                                  %prec PRE_MINUSMINUS
                   | PLUS expression                                        %prec UNARY_PLUS
                   | MINUS expression                                       %prec UNARY_MINUS
                   | LNOT expression
                   | NOT expression
                   | LPAREN type RPAREN expression                          %prec UNARY_TIMES
                   | TIMES expression                                       %prec UNARY_TIMES
                   | AND expression                                         %prec UNARY_AND
                   | SIZEOF expression
                   | SIZEOF LPAREN type RPAREN                              %prec SIZEOF
                   | expression TIMES expression
                   | expression DIVIDE expression
                   | expression MOD expression
                   | expression PLUS expression
                   | expression MINUS expression
                   | expression LSHIFT expression
                   | expression RSHIFT expression
                   | expression LT expression
                   | expression LE expression
                   | expression GT expression
                   | expression GE expression
                   | expression EQ expression
                   | expression NE expression
                   | expression AND expression
                   | expression XOR expression
                   | expression OR expression
                   | expression LAND expression
                   | expression LOR expression
                   | expression CONDOP expression_list COLON expression
                   | expression EQUALS expression
                   | expression TIMESEQUAL expression
                   | expression DIVEQUAL expression
                   | expression MODEQUAL expression
                   | expression PLUSEQUAL expression
                   | expression MINUSEQUAL expression
                   | expression LSHIFTEQUAL expression
                   | expression RSHIFTEQUAL expression
                   | expression ANDEQUAL expression
                   | expression OREQUAL expression
                   | expression XOREQUAL expression
                   | STATIC_CAST LT type GT LPAREN expression RPAREN        %prec TEMPLATEGT
                   | DYNAMIC_CAST LT type GT LPAREN expression RPAREN       %prec TEMPLATEGT
                   | REINTERPRET_CAST LT type GT LPAREN expression RPAREN   %prec TEMPLATEGT
                   | CONST_CAST LT type GT LPAREN expression RPAREN         %prec TEMPLATEGT
    """
    pass


def p_expression_list(p):
    """
        expression_list : expression_list COMMA expression_list
                        | expression                                        %prec PRIO0
    """

def p_expression_list_opt(p):
    """
        expression_list_opt : expression_list
                            |
    """
