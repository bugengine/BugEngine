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
        statement : expression SEMI
    """
    pass


def p_variable_declaration_opt(p):
    """
        variable_declaration_opt : variable_declaration
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
    pass


def p_pop_for_scope(p):
    """
        pop_for_scope :
    """
    pass


def p_statement_for(p):
    """
        statement : FOR push_for_scope LPAREN variable_declaration_opt SEMI expression_opt SEMI expression_opt RPAREN statement pop_for_scope
    """
    pass

def p_statement_flow(p):
    """
        statement : BREAK SEMI
                  | CONTINUE SEMI
    """
    pass
