def p_statement_list_end(p):
    # type: (YaccProduction) -> None
    """
        statement_list :
    """
    pass


def p_statement_list(p):
    # type: (YaccProduction) -> None
    """
        statement_list : statement statement_list
    """
    pass


def p_statement_block(p):
    # type: (YaccProduction) -> None
    """
        statement : statement_block
    """
    pass


def p_statement_block_1(p):
    # type: (YaccProduction) -> None
    """
        statement_block : LBRACE BRACE_MARKER statement_list RBRACE
    """
    pass


def p_statement_block_error(p):
    # type: (YaccProduction) -> None
    """
        statement_block : LBRACE BRACE_MARKER error RBRACE
    """
    pass


def p_statement_typedef(p):
    # type: (YaccProduction) -> None
    """
        statement : typedef SEMI
    """
    pass


def p_statement_variable_declaration(p):
    # type: (YaccProduction) -> None
    """
        statement : variable_declaration SEMI
    """
    pass


def p_statement_expression(p):
    # type: (YaccProduction) -> None
    """
        statement : declaration_specifier_list expression SEMI
    """
    for s in p[1]:
        p.lexer.logger.C1000(s.position, s.specifier)


def p_variable_declaration_opt(p):
    # type: (YaccProduction) -> None
    """
        variable_declaration_opt : variable_declaration
                                 | declaration_specifier_list expression_list_opt
    """
    pass


def p_expression_opt(p):
    # type: (YaccProduction) -> None
    """
        expression_opt : expression_list
                       |
    """
    pass


def p_push_for_scope(p):
    # type: (YaccProduction) -> None
    """
        push_for_scope :
    """


def p_pop_for_scope(p):
    # type: (YaccProduction) -> None
    """
        pop_for_scope :
    """


def p_statement_for(p):
    # type: (YaccProduction) -> None
    """
        statement : FOR push_for_scope LPAREN variable_declaration_opt SEMI expression_opt SEMI expression_opt RPAREN statement pop_for_scope
    """
    pass


def p_statement_for_error(p):
    # type: (YaccProduction) -> None
    """
        statement : FOR push_for_scope LPAREN error SEMI expression_opt SEMI expression_opt RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN variable_declaration_opt SEMI error SEMI expression_opt RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN variable_declaration_opt SEMI expression_opt SEMI error RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN error SEMI error SEMI expression_opt RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN error SEMI expression_opt SEMI error RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN variable_declaration_opt SEMI error SEMI error RPAREN statement pop_for_scope
                  | FOR push_for_scope LPAREN error SEMI error SEMI error RPAREN statement pop_for_scope
    """


def p_statement_flow(p):
    # type: (YaccProduction) -> None
    """
        statement : BREAK SEMI
                  | CONTINUE SEMI
    """
    pass


def p_switch_statement(p):
    # type: (YaccProduction) -> None
    """
        switch_statement : DEFAULT COLON statement_list
                         | CASE expression COLON statement_list
    """
    pass


def p_switch_statements(p):
    # type: (YaccProduction) -> None
    """
        switch_statements : switch_statement switch_statements
                          |
    """
    pass


def p_statement_switch(p):
    # type: (YaccProduction) -> None
    """
        statement : SWITCH LPAREN expression RPAREN LBRACE BRACE_MARKER switch_statements RBRACE
    """
    pass


def p_statement_if(p):
    # type: (YaccProduction) -> None
    """
        statement : IF LPAREN expression RPAREN statement ELSE statement
                  | IF LPAREN expression RPAREN statement                   %prec IFX
    """


def p_statement_if_error(p):
    # type: (YaccProduction) -> None
    """
        statement : IF LPAREN error RPAREN statement ELSE statement
                  | IF LPAREN error RPAREN statement                        %prec IFX
    """


def p_statement_return(p):
    # type: (YaccProduction) -> None
    """
        statement : RETURN SEMI
                  | RETURN expression SEMI
    """


def p_statement_while(p):
    # type: (YaccProduction) -> None
    """
        statement : WHILE LPAREN expression RPAREN statement
    """


def p_statement_while_error(p):
    # type: (YaccProduction) -> None
    """
        statement : WHILE LPAREN error RPAREN statement
    """


def p_statemen_do_while(p):
    # type: (YaccProduction) -> None
    """
        statement : DO statement WHILE LPAREN expression RPAREN SEMI
    """


def p_statemen_do_while_error(p):
    # type: (YaccProduction) -> None
    """
        statement : DO statement WHILE LPAREN error RPAREN SEMI
    """


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from ply.yacc import YaccProduction