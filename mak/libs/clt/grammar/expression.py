from .. import cl_ast


def p_value_float(p):
    """
        expression : FLOAT_CONST
    """
    type = cl_ast.types.Builtin('float', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    if p[1][-1] == 'f':
        p[1] = p[1][:-1]
    p[0] = cl_ast.values.Value(type, float(p[1]), p.position(1))


def p_value_float_hex(p):
    """
        expression : HEX_FLOAT_CONST
    """
    type = cl_ast.types.Builtin('float', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', position)
    p[0] = cl_ast.values.Value(type, float.fromhex(p[1]), p.position(1))


def int_to_value(string_value, base, position):
    type = 'i32'
    while string_value[-1] in ('l', 'u'):
        if string_value[-1] == 'l':
            type = type[0] + '64'
        elif string_value[-1] == 'u':
            type = 'u' + type[1:]
        string_value = string_value[:-1]
    value = int(string_value, base)
    type = cl_ast.types.Builtin(type, position)
    type = cl_ast.types.Type(type, position)
    type.add_modifier('const', position)
    return cl_ast.values.Value(type, value, position)


def p_value_int_10(p):
    """
        expression : INT_CONST_DEC
    """
    p[0] = int_to_value(p[1], 10, p.position(1))


def p_value_int_8(p):
    """
        expression : INT_CONST_OCT
    """
    p[0] = int_to_value(p[1], 8, p.position(1))


def p_value_int_16(p):
    """
        expression : INT_CONST_HEX
    """
    p[0] = int_to_value(p[1][2:], 16, p.position(1))


def p_value_int_2(p):
    """
        expression : INT_CONST_BIN
    """
    p[0] = int_to_value(p[1][2:], 2, p.position(1))


def p_value_char(p):
    """
        expression : CHAR_CONST
    """
    type = cl_ast.types.Builtin('i8', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    str = eval(p[1])
    p[0] = cl_ast.values.Value(type, str, p.position(1))


def p_value_wchar(p):
    """
        expression : WCHAR_CONST
    """
    type = cl_ast.types.Builtin('i8', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    str = eval(p[1][1:])
    p[0] = cl_ast.values.Value(type, str, p.position(1))


def p_value_string_literal(p):
    """
        expression : STRING_LITERAL
    """
    type = cl_ast.types.Builtin('i8', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    str = eval(p[1])
    str = str.encode('utf8')
    p[0] = cl_ast.values.Value(type, str, p.position(1))


def p_value_wstring_literal(p):
    """
        expression : WSTRING_LITERAL
    """
    type = cl_ast.types.Builtin('i8', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    str = eval(p[1][1:])
    str = str.encode('utf32')
    p[0] = cl_ast.values.Value(type, str, p.position(1))


def p_value_true(p):
    """
        expression : TRUE
                   | FALSE
    """
    type = cl_ast.types.Builtin('bool', p.position(1))
    type = cl_ast.types.Type(type, p.position(1))
    type.add_modifier('const', p.position(1))
    p[0] = cl_ast.values.Value(type, p[1] == 'true', p.position(1))


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
        p[0] = cl_ast.values.DependentValueName(p[1])
    else:
        p[0] = p[1].target
        if not p[0]:
            p.lexer._error('Unknown object: %s' % ('::'.join(p[1].name)), p[1].position)



precedence = (
    ('nonassoc','IFX'),
    ('nonassoc','ELSE'),
    ('left',    'PRIO14'),
    ('right',   'PRIO13'),
    ('left',    'PRIO12'),
    ('left',    'PRIO11'),
    ('left',    'PRIO10'),
    ('left',    'PRIO9'),
    ('left',    'PRIO8'),
    ('left',    'PRIO7'),
    ('nonassoc','PRIO6'),
    ('left',    'PRIO5'),
    ('left',    'PRIO4'),
    ('left',    'PRIO3'),
    ('right',   'PRIO2'),
    ('left',    'PRIO1'),
    ('left',    'PRIO0'),
    ('left',    'SCOPE'),
    ('nonassoc','TYPEMODIFIER'),
    ('nonassoc','TEMPLATEGT'),
)


def p_operator_expr(p):
    """
        expression : LPAREN expression_list RPAREN                          %prec PRIO0
                   | expression PLUSPLUS                                    %prec PRIO1
                   | expression MINUSMINUS                                  %prec PRIO1
                   | expression LPAREN expression_list RPAREN               %prec PRIO1
                   | expression LBRACKET expression_list RBRACKET           %prec PRIO1
                   | expression PERIOD object_name                          %prec PRIO1
                   | expression ARROW object_name                           %prec PRIO1
                   | PLUSPLUS expression                                    %prec PRIO2
                   | MINUSMINUS expression                                  %prec PRIO2
                   | PLUS expression                                        %prec PRIO2
                   | MINUS expression                                       %prec PRIO2
                   | LNOT expression                                        %prec PRIO2
                   | NOT expression                                         %prec PRIO2
                   | LPAREN type RPAREN expression                          %prec PRIO2
                   | TIMES expression                                       %prec PRIO2
                   | AND expression                                         %prec PRIO2
                   | SIZEOF expression                                      %prec PRIO2
                   | SIZEOF LPAREN type RPAREN                              %prec PRIO2
                   | expression TIMES expression                            %prec PRIO3
                   | expression DIVIDE expression                           %prec PRIO3
                   | expression MOD expression                              %prec PRIO3
                   | expression PLUS expression                             %prec PRIO4
                   | expression MINUS expression                            %prec PRIO4
                   | expression LSHIFT expression                           %prec PRIO5
                   | expression RSHIFT expression                           %prec PRIO5
                   | expression LT expression                               %prec PRIO6
                   | expression LE expression                               %prec PRIO6
                   | expression GT expression                               %prec PRIO6
                   | expression GE expression                               %prec PRIO6
                   | expression EQ expression                               %prec PRIO7
                   | expression NE expression                               %prec PRIO7
                   | expression AND expression                              %prec PRIO8
                   | expression XOR expression                              %prec PRIO9
                   | expression OR expression                               %prec PRIO10
                   | expression LAND expression                             %prec PRIO11
                   | expression LOR expression                              %prec PRIO12
                   | expression CONDOP expression_list COLON expression     %prec PRIO13
                   | expression EQUALS expression                           %prec PRIO13
                   | expression TIMESEQUAL expression                       %prec PRIO13
                   | expression DIVEQUAL expression                         %prec PRIO13
                   | expression MODEQUAL expression                         %prec PRIO13
                   | expression PLUSEQUAL expression                        %prec PRIO13
                   | expression MINUSEQUAL expression                       %prec PRIO13
                   | expression LSHIFTEQUAL expression                      %prec PRIO13
                   | expression RSHIFTEQUAL expression                      %prec PRIO13
                   | expression ANDEQUAL expression                         %prec PRIO13
                   | expression OREQUAL expression                          %prec PRIO13
                   | expression XOREQUAL expression                         %prec PRIO13
                   | STATIC_CAST LT type GT LPAREN expression RPAREN        %prec TEMPLATEGT
                   | DYNAMIC_CAST LT type GT LPAREN expression RPAREN       %prec TEMPLATEGT
                   | REINTERPRET_CAST LT type GT LPAREN expression RPAREN   %prec TEMPLATEGT
                   | CONST_CAST LT type GT LPAREN expression RPAREN         %prec TEMPLATEGT
    """
    pass


def p_expression_list(p):
    """
        expression_list : expression_list COMMA expression_list         %prec PRIO14
                        | expression                                    %prec PRIO0
    """
