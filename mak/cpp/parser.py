import sys
try:
	import cpp.lexer as lexer
	import ply.yacc as yacc
	import cpp.tokens
	from cpp.tokens import *
except:
	import lexer
	import yacc

class NamespaceBegin(yacc.Nonterm):
	"%nonterm"
	def namespace_named(self, namespace, name, LBRACE):
		"%reduce NAMESPACE ID LBRACE"
		print(name.value)
	def namespace_unnamed(self, namespace, LBRACE):
		"%reduce NAMESPACE LBRACE"

class Expr(yacc.Nonterm):
	"%nonterm"
	def expr_namespace(self, namespace_begin, exprs, rbrace):
		"%reduce NamespaceBegin Exprs RBRACE"

class Exprs(yacc.Nonterm):
	"%nonterm"
	def expr_list(self, exprs, expr):
		"%reduce Expr Exprs"
	def empty(self):
		"%reduce"

class Header(yacc.Nonterm):
	"%start"

	def exprs(self, exprs):
		"%reduce Exprs"



spec = yacc.Spec(sys.modules[__name__],
				pickleFile="cpp.pickle",
				logFile="cpp.log",
				graphFile="cpp.dot",
				verbose=True)

class Parser(yacc.Lr):
	def __init__(self):
		yacc.Lr.__init__(self, spec)

	def parse(self, input, lexer):
		lexer.input(input)
		t=lexer.token()
		while t:
			tok = cpp.tokens.__dict__[t.type](self)
			tok.value = t.value
			tok.lineno = t.lineno
			self.token(tok)
			t=lexer.token()
		self.eoi()
