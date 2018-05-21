def p_value(p):
    """
        expression : FLOAT_CONST
                   | HEX_FLOAT_CONST
                   | INT_CONST_DEC
                   | INT_CONST_OCT
                   | INT_CONST_HEX
                   | INT_CONST_BIN
                   | CHAR_CONST
                   | WCHAR_CONST
                   | STRING_LITERAL
                   | WSTRING_LITERAL
                   | TRUE
                   | FALSE
                   | THIS
    """
    p[0] = p[1]


def p_value_object(p):
    """
        expression : object_name
    """
    if not p[1][2]:
        p.lexer._error('Unknown object: %s' % p[1][0], p.position(1))
    p[0] = p[1]



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
