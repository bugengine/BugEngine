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
                   | VARIABLE_ID
                   | VARIABLE_ID_SHADOW
    """
    pass


def p_any_id(p):
    """
        any_id : ID
               | STRUCT_ID
               | TYPENAME_ID
               | NAMESPACE_ID
               | METHOD_ID
               | VARIABLE_ID
               | STRUCT_ID_SHADOW
               | TYPENAME_ID_SHADOW
               | NAMESPACE_ID_SHADOW
               | METHOD_ID_SHADOW
               | VARIABLE_ID_SHADOW
    """
    p[0] = p[1]

precedence = (
    ('left',   'PRIO12'),
    ('right',   'PRIO11'),
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
)

def p_operator_expr(p):
    """
        expression : LPAREN expression_list RPAREN                      %prec PRIO0
                   | expression PLUSPLUS                                %prec PRIO1
                   | expression MINUSMINUS                              %prec PRIO1
                   | expression LBRACKET expression_list RBRACKET       %prec PRIO1
                   | expression PERIOD any_id                           %prec PRIO1
                   | expression ARROW any_id                            %prec PRIO1
                   | PLUSPLUS expression                                %prec PRIO2
                   | MINUSMINUS expression                              %prec PRIO2
                   | PLUS expression                                    %prec PRIO2
                   | MINUS expression                                   %prec PRIO2
                   | LNOT expression                                    %prec PRIO2
                   | NOT expression                                     %prec PRIO2
                   | LPAREN type RPAREN expression                      %prec PRIO2
                   | TIMES expression                                   %prec PRIO2
                   | AND expression                                     %prec PRIO2
                   | SIZEOF expression                                  %prec PRIO2
                   | SIZEOF LPAREN type RPAREN                          %prec PRIO2
                   | expression TIMES expression                        %prec PRIO3
                   | expression DIVIDE expression                       %prec PRIO3
                   | expression MOD expression                          %prec PRIO3
                   | expression PLUS expression                         %prec PRIO4
                   | expression MINUS expression                        %prec PRIO4
                   | expression LSHIFT expression                       %prec PRIO5
                   | expression RSHIFT expression                       %prec PRIO5
                   | expression LT expression                           %prec PRIO6
                   | expression LE expression                           %prec PRIO6
                   | expression GT expression                           %prec PRIO6
                   | expression GE expression                           %prec PRIO6
                   | expression EQ expression                           %prec PRIO7
                   | expression NE expression                           %prec PRIO7
                   | expression AND expression                          %prec PRIO8
                   | expression XOR expression                          %prec PRIO9
                   | expression OR expression                           %prec PRIO10
                   | expression CONDOP expression_list COLON expression %prec PRIO11
                   | expression EQUALS expression                       %prec PRIO11
                   | expression TIMESEQUAL expression                   %prec PRIO11
                   | expression DIVEQUAL expression                     %prec PRIO11
                   | expression MODEQUAL expression                     %prec PRIO11
                   | expression PLUSEQUAL expression                    %prec PRIO11
                   | expression MINUSEQUAL expression                   %prec PRIO11
                   | expression LSHIFTEQUAL expression                  %prec PRIO11
                   | expression RSHIFTEQUAL expression                  %prec PRIO11
                   | expression ANDEQUAL expression                     %prec PRIO11
                   | expression OREQUAL expression                      %prec PRIO11
                   | expression XOREQUAL expression                     %prec PRIO11
    """
    pass

def p_expression_list(p):
    """
        expression_list : expression_list COMMA expression_list         %prec PRIO12
                        | expression                                    %prec PRIO0
    """
