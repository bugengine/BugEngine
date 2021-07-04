import os
import glrp


class Parser1(glrp.Parser):
    class Lexer(glrp.Lexer):
        @glrp.token(r'a', 'a')
        @glrp.token(r'b', 'b')
        @glrp.token(r'c', 'c')
        def tok(self, token):
            # type: (glrp.Token) -> glrp.Token
            return token

    @glrp.rule("prog : A")
    def p_prog(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("A : a B ")
    @glrp.rule("A : a a B ")
    @glrp.rule("A : a a a B ")
    #@glrp.rule("A : B c")
    def p_A(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("B [prec:right,0][split]: a a b")
    @glrp.rule("B [prec:right,0][split]: a b")
    @glrp.rule("B [prec:right,0][split]: b")
    def p_B(self, p):
        # type: (glrp.Production) -> None
        pass

    def __init__(self):
        # type: ()->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(self, self.lexer, 'prog', '.')


class Parser2(glrp.Parser):
    class Lexer(glrp.Lexer):
        @glrp.token(r'identifier', 'identifier')
        @glrp.token(r'typename', 'typename')
        @glrp.token(r';', ';')
        @glrp.token(r'\(', '(')
        @glrp.token(r'\)', ')')
        @glrp.token(r'\+', '+')
        @glrp.token(r'\=', '=')
        def tok(self, token):
            # type: (glrp.Token) -> glrp.Token
            return token

    @glrp.rule("prog :")
    @glrp.rule("prog : prog stmt")
    def p_prog(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("stmt : expr ';'")
    @glrp.rule("stmt : decl")
    def p_stmt(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("expr : 'identifier'")
    @glrp.rule("expr : 'typename' '(' expr ')'")
    @glrp.rule("expr[prec:left,0] : expr [prec:left,0]'+' expr")
    @glrp.rule("expr[prec:right,1] : expr [prec:right,1]'=' expr")
    def p_expr(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("decl : 'typename' declarator")
    @glrp.rule("decl : 'typename' declarator '=' expr")
    def p_decl(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("declarator : 'identifier'")
    @glrp.rule("declarator : '(' declarator ')'")
    def p_declarator(self, p):
        # type: (glrp.Production) -> None
        pass

    def __init__(self):
        # type: ()->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(self, self.lexer, 'prog', '.')


class Parser3(glrp.Parser):
    class Lexer(glrp.Lexer):
        @glrp.token(r'if', 'if')
        @glrp.token(r'else', 'else')
        @glrp.token(r'\(', '(')
        @glrp.token(r'\)', ')')
        @glrp.token(r'\{', '{')
        @glrp.token(r'\}', '}')
        @glrp.token(r'\;', ';')
        @glrp.token(r'identifier', 'identifier')
        def tok(self, token):
            # type: (glrp.Token) -> glrp.Token
            return token

    @glrp.rule("prog : stmt")
    def p_prog(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("stmt : expr ';'")
    @glrp.rule("stmt : if_stmt")
    def p_stmt(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("expr : 'identifier'")
    def p_expr(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("if_stmt : 'if' '(' expr ')' stmt")
    @glrp.rule("if_stmt : 'if' '(' expr ')' stmt 'else' stmt")
    def p_if_stmt(self, p):
        # type: (glrp.Production) -> None
        pass

    def __init__(self):
        # type: ()->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(self, self.lexer, 'prog', '.')


class Parser4(glrp.Parser):
    class Lexer(glrp.Lexer):
        @glrp.token(r'id', 'id')
        @glrp.token(r'\,', ',')
        def tok(self, token):
            # type: (glrp.Token) -> glrp.Token
            return token

    @glrp.rule("s : a id")
    def p_s(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("a : expr")
    def p_a(self, p):
        # type: (glrp.Production) -> None
        pass

    @glrp.rule("expr : ")
    @glrp.rule("expr : expr 'id' ','")
    def p_expr(self, p):
        # type: (glrp.Production) -> None
        pass

    def __init__(self):
        # type: ()->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(self, self.lexer, 's', '.')