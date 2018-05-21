from .. import cl_ast


def p_statement_list_end(p):
    """
        statement_list :
    """
    pass


def p_statement_list(p):
    """
        statement_list : statement statement_list
    """
    pass


def p_statement_error(p):
    """
        statement : error SEMI
    """
    pass


def p_statement_block(p):
    """
        statement : statement_block
    """
    pass


def p_statement_block_1(p):
    """
        statement_block : LBRACE statement_list RBRACE
    """
    pass


def p_statement_block_error(p):
    """
        statement_block : LBRACE error RBRACE
    """
    pass


def p_statement_variable_declaration(p):
    """
        statement : variable_declaration SEMI
    """
    pass


def p_statement_expression(p):
    """
        statement : declaration_specifier_list expression SEMI
    """
    for s in p[1]:
        p.lexer._error('Unexpected specifier: %s' % s.specifier, s.position)


def p_variable_declaration_opt(p):
    """
        variable_declaration_opt : variable_declaration
                                 | declaration_specifier_list expression_list
                                 |
    """
    pass


def p_expression_opt(p):
    """
        expression_opt : expression_list
                       |
    """
    pass


def p_push_for_scope(p):
    """
        push_for_scope :
    """
    p[0] = cl_ast.ForStatement(p.position(-1))
    p.lexer.push_scope(p[0])


def p_pop_for_scope(p):
    """
        pop_for_scope :
    """
    p.lexer.pop_scope()


def p_statement_for(p):
    """
        statement : FOR push_for_scope LPAREN variable_declaration_opt SEMI expression_opt SEMI expression_opt RPAREN statement pop_for_scope
    """
    pass


def p_statement_for_error(p):
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
    """
        statement : BREAK SEMI
                  | CONTINUE SEMI
    """
    pass


def p_switch_statement(p):
    """
        switch_statement : DEFAULT COLON statement_list
                         | CASE expression COLON statement_list
    """
    pass

def p_switch_statements(p):
    """
        switch_statements : switch_statement switch_statements
                          |
    """
    pass


def p_statement_switch(p):
    """
        statement : SWITCH LPAREN expression RPAREN LBRACE switch_statements RBRACE
    """
    pass

def p_statement_if(p):
    """
        statement : IF LPAREN expression RPAREN statement ELSE statement
                  | IF LPAREN expression RPAREN statement                   %prec IFX
    """


def p_statement_if_error(p):
    """
        statement : IF LPAREN error RPAREN statement ELSE statement
                  | IF LPAREN error RPAREN statement                        %prec IFX
    """



def p_statement_return(p):
    """
        statement : RETURN SEMI
                  | RETURN expression SEMI
    """


def p_statement_while(p):
    """
        statement : WHILE LPAREN expression RPAREN statement
    """


def p_statement_while_error(p):
    """
        statement : WHILE LPAREN error RPAREN statement
    """


def p_statemen_do_while(p):
    """
        statement : DO statement WHILE LPAREN expression RPAREN SEMI
    """


def p_statemen_do_while_error(p):
    """
        statement : DO statement WHILE LPAREN error RPAREN SEMI
    """
