from ..cl_ast import specifiers


def p_declaration_specifier(p):
    """
        declaration_specifier : STATIC
                              | INLINE
                              | EXPLICIT
                              | VIRTUAL
    """
    p[0] = p[1]


def p_declaration_specifier_list_end(p):
    """
        declaration_specifier_list :
    """
    p[0] = []


def p_declaration_specifier_list(p):
    """
        declaration_specifier_list : declaration_specifier_list declaration_specifier
    """
    p[0] = p[1] + [specifiers.Specifier(p.lexer, p.position(2), p[2])]


def p_external_declarations_end(p):
    """
        external_declarations :
    """


def p_external_declarations(p):
    """
        external_declarations : external_declaration external_declarations
    """


def p_external_declarations_namespace(p):
    """
        external_declarations : namespace_declaration external_declarations
    """


def p_external_declaration_empty(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list verify_template_stack_0 SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_type(p):
    """
        external_declaration : template_specifier_opt declaration_specifier_list typedecl SEMI
        external_declaration : template_specifier_opt declaration_specifier_list type_definition SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)
    if p.lexer.template_stack:
        p.lexer.finalize_template_stack()


def p_external_declaration_variable(p):
    """
        external_declaration : template_specifier_opt variable_declaration SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_method(p):
    """
        external_declaration : template_specifier_opt method_declaration SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_method_definition(p):
    """
        external_declaration : template_specifier_opt method_definition
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_external_declaration_using(p):
    """
        external_declaration : USING object_name SEMI
                             | USING NAMESPACE object_name SEMI
    """


def p_external_declaration_typedef(p):
    """
        external_declaration : typedef SEMI
    """


def p_external_declaration_error(p):
    """
        external_declaration : template_specifier_opt error SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)
    if p.lexer.template_stack:
        p.lexer.finalize_template_stack()


def p_external_declaration_error_block(p):
    """
        find_rbrace :
    """
    assert p.lexer.last_token.type == 'LBRACE', p.lexer.last_token
    token = p.lexer.last_token
    t = p.lexer.token()
    depth = 1
    while t:
        if t.type == 'RBRACE':
            depth -= 1
            if depth == 0:
                break
        elif t.type == 'LBRACE':
            depth += 1
        t = p.lexer.token()
    p.lexer.last_token = token
    p.parser.errok()


def p_error_declaration_error_block(p):
    """
        external_declaration : template_specifier_opt error LBRACE find_rbrace BRACE_MARKER
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)
    if p.lexer.template_stack:
        p.lexer.finalize_template_stack()