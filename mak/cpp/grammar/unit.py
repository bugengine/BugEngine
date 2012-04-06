import cpp.yacc as yacc
import rtti

class Expr(yacc.Nonterm):
	"%nonterm"
	def expr_namespace(self, namespace):
		"%reduce Namespace"
		self.value = namespace.value
	def expr_class(self, cls):
		"%reduce ClassDef"
		self.value = cls.value
	def expr_using(self, using):
		"%reduce Using"
		self.value = None

class Exprs(yacc.Nonterm):
	"%nonterm"
	def expr_list(self, expr, exprs):
		"%reduce Expr Exprs"
		if expr.value:
			self.values = [expr.value]+exprs.values
		else:
			self.values = exprs.values[:]
	def empty(self):
		"%reduce"
		self.values = []

class Unit(yacc.Nonterm):
	"%start"

	def __init__(self, parser):
		yacc.Nonterm.__init__(self, parser)
		self.result = parser.root

	def exprs(self, exprs):
		"%reduce Exprs"
		for obj in exprs.values:
			self.result.addObject(obj)

