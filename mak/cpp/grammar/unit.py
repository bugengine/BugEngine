import cpp.yacc as yacc

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

class Unit(yacc.Nonterm):
	"%start"

	def exprs(self, exprs):
		"%reduce Exprs"

