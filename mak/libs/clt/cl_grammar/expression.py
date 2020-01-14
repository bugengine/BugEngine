from ..cl_ast import ast_values
from typing import TYPE_CHECKING


def p_value_constant(p):
    # type: (YaccProduction) -> None
    """
        expression : FLOAT_CONST
                   | HEX_FLOAT_CONST
                   | INT_CONST_DEC
                   | INT_CONST_OCT
                   | INT_CONST_HEX
                   | INT_CONST_BIN
                   | CHAR_CONST
                   | WCHAR_CONST
    """
    p[0] = ast_values.Constant(
        p.lexer, p.position(1), getattr(p.slice[1], 'constant_type'), getattr(p.slice[1], 'constant_value')
    )


def p_value_string_literal(p):
    # type: (YaccProduction) -> None
    """
        expression : STRING_LITERAL
    """


def p_value_wstring_literal(p):
    # type: (YaccProduction) -> None
    """
        expression : WSTRING_LITERAL
    """


def p_value_true(p):
    # type: (YaccProduction) -> None
    """
        expression : TRUE
                   | FALSE
    """
    p[0] = ast_values.Constant(p.lexer, p.position(1), p.lexer.base_types['bool'], p[1] == "true")


def p_value_this(p):
    # type: (YaccProduction) -> None
    """
        expression : THIS
    """
    # TODO


def p_value_object(p):
    # type: (YaccProduction) -> None
    """
        expression : object_name
    """
    #p.lexer.error("'%s' was not declared in this scope" % p[1][0], p[1][0].position)
    p[0] = p[1][0].target


def p_parameterlist_opt(p):
    # type: (YaccProduction) -> None
    """
        parameter_list_opt : LPAREN expression_list_opt RPAREN              %prec PRIO0
                           |                                                %prec AND
    """


def p_binary_operator_expr(p):
    # type: (YaccProduction) -> None
    """
        expression : expression TIMES expression
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
    """
    p[0] = ast_values.BinaryOperation(p.lexer, p.position(2), p[2], p[1], p[3])


def p_operator_expr(p):
    # type: (YaccProduction) -> None
    """
        expression : LPAREN expression_list RPAREN
                   | expression PLUSPLUS                                    %prec PRIO1
                   | expression MINUSMINUS                                  %prec PRIO1
                   | expression LPAREN expression_list_opt RPAREN           %prec PRIO1
                   | type LPAREN expression_list_opt RPAREN                 %prec PRIO1
                   | expression LBRACKET expression_list RBRACKET           %prec PRIO1
                   | expression PERIOD object_name                          %prec PRIO1
                   | expression ARROW object_name                           %prec PRIO1
                   | expression PERIOD special_method_name                  %prec PRIO1
                   | expression ARROW special_method_name                   %prec PRIO1
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
                   | expression CONDOP expression_list COLON expression
                   | STATIC_CAST LT type GT LPAREN expression RPAREN        %prec TEMPLATEGT
                   | DYNAMIC_CAST LT type GT LPAREN expression RPAREN       %prec TEMPLATEGT
                   | REINTERPRET_CAST LT type GT LPAREN expression RPAREN   %prec TEMPLATEGT
                   | CONST_CAST LT type GT LPAREN expression RPAREN         %prec TEMPLATEGT
                   | NEW parameter_list_opt type parameter_list_opt         %prec TEMPLATEGT
                   | DELETE expression
    """
    pass


def p_expression_list(p):
    # type: (YaccProduction) -> None
    """
        expression_list : expression_list COMMA expression_list
                        | expression                                        %prec PRIO0
    """


def p_expression_list_opt(p):
    # type: (YaccProduction) -> None
    """
        expression_list_opt : expression_list
                            |
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction