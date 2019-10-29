

def p_value_float(p):
    """
        expression : FLOAT_CONST
    """


def p_value_float_hex(p):
    """
        expression : HEX_FLOAT_CONST
    """


def p_value_int_10(p):
    """
        expression : INT_CONST_DEC
    """


def p_value_int_8(p):
    """
        expression : INT_CONST_OCT
    """


def p_value_int_16(p):
    """
        expression : INT_CONST_HEX
    """


def p_value_int_2(p):
    """
        expression : INT_CONST_BIN
    """


def p_value_char(p):
    """
        expression : CHAR_CONST
    """


def p_value_wchar(p):
    """
        expression : WCHAR_CONST
    """


def p_value_string_literal(p):
    """
        expression : STRING_LITERAL
    """


def p_value_wstring_literal(p):
    """
        expression : WSTRING_LITERAL
    """


def p_value_true(p):
    """
        expression : TRUE
                   | FALSE
    """


def p_value_this(p):
    """
        expression : THIS
    """


def p_value_object(p):
    """
        expression : object_name
    """


def p_operator_expr(p):
    """
        expression : LPAREN expression_list RPAREN
                   | expression PLUSPLUS                                    %prec PRIO1
                   | expression MINUSMINUS                                  %prec PRIO1
                   | expression LPAREN expression_list_opt RPAREN           %prec PRIO1
                   | type LPAREN expression_list_opt RPAREN                 %prec PRIO1
                   | expression LBRACKET expression_list RBRACKET           %prec PRIO1
                   | expression PERIOD object_name                          %prec PRIO1
                   | expression ARROW object_name                           %prec PRIO1
                   | expression PERIOD NOT object_name                      %prec PRIO1
                   | expression ARROW NOT object_name                       %prec PRIO1
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
